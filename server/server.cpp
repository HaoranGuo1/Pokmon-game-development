/*
    server.cpp
    Pokemon Battle HTTP Server
    Exposes REST API for the web frontend to interact with game logic.
*/

#define CPPHTTPLIB_THREAD_POOL_COUNT 1
#include "httplib.h"
#include "../src/pokemon.hpp"
#include "../src/move.hpp"
#include <string>
#include <vector>
#include <mutex>
#include <sstream>
#include <fstream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <iostream>

// ─────────────────────────── Game State ────────────────────────────

struct GameState {
    std::unique_ptr<Pokemon> p1;
    std::unique_ptr<Pokemon> p2;
    bool p1_used_heal = false;
    bool p2_used_heal = false;
    int  turn         = 0;   // 0 = player1, 1 = player2
    bool started      = false;
    bool over         = false;
    std::string winner;
    std::vector<std::string> battle_log;
};

static GameState g_state;
static std::mutex g_mutex;

// ─────────────────────────── JSON Helpers ───────────────────────────

static std::string json_escape(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            default:   out += c;
        }
    }
    return out;
}

static std::string move_to_json(const Move& m) {
    return std::string("{")
        + "\"name\":\""       + json_escape(m.name)            + "\","
        + "\"type\":\""       + json_escape(m.type)            + "\","
        + "\"base_damage\":"  + std::to_string(m.base_damage)  + ","
        + "\"uses_left\":"    + std::to_string(m.uses_left)    + "}";
}

static std::string pokemon_to_json(const Pokemon& p) {
    std::string moves_json = "[";
    for (int i = 0; i < p.get_num_moves(); i++) {
        if (i > 0) moves_json += ",";
        moves_json += move_to_json(p.get_move(i));
    }
    moves_json += "]";

    return std::string("{")
        + "\"name\":\""      + json_escape(p.get_name())           + "\","
        + "\"type\":\""      + json_escape(p.get_type())           + "\","
        + "\"hp\":"          + std::to_string(p.get_hp())          + ","
        + "\"max_hp\":"      + std::to_string(p.get_max_hp())      + ","
        + "\"is_fainted\":"  + (p.is_fainted() ? "true" : "false") + ","
        + "\"moves\":"       + moves_json                          + "}";
}

static std::string log_to_json(const std::vector<std::string>& log) {
    std::string out = "[";
    for (size_t i = 0; i < log.size(); i++) {
        if (i > 0) out += ",";
        out += "\"" + json_escape(log[i]) + "\"";
    }
    return out + "]";
}

static std::string state_to_json(const GameState& g) {
    if (!g.started) {
        return "{\"started\":false}";
    }
    return std::string("{")
        + "\"started\":true,"
        + "\"over\":"          + (g.over ? "true" : "false")         + ","
        + "\"winner\":\""      + json_escape(g.winner)               + "\","
        + "\"turn\":"          + std::to_string(g.turn)              + ","
        + "\"p1\":"            + pokemon_to_json(*g.p1)              + ","
        + "\"p2\":"            + pokemon_to_json(*g.p2)              + ","
        + "\"p1_used_heal\":"  + (g.p1_used_heal ? "true" : "false") + ","
        + "\"p2_used_heal\":"  + (g.p2_used_heal ? "true" : "false") + ","
        + "\"log\":"           + log_to_json(g.battle_log)
        + "}";
}

// ─────────────────────── Simple JSON Parser ─────────────────────────

static int extract_int(const std::string& body, const std::string& key) {
    std::string search = "\"" + key + "\":";
    size_t pos = body.find(search);
    if (pos == std::string::npos) return -1;
    pos += search.size();
    // skip whitespace
    while (pos < body.size() && body[pos] == ' ') pos++;
    try {
        return std::stoi(body.substr(pos));
    } catch (...) {
        return -1;
    }
}

// ───────────────────────── CORS Helper ──────────────────────────────

static void add_cors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin",  "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

// ────────────────── Capture cout → battle log ────────────────────────

// Redirects std::cout to a buffer, runs fn(), restores cout,
// then splits lines into battle_log.
template<typename Fn>
static void capture_cout(std::vector<std::string>& battle_log, Fn fn) {
    std::ostringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    fn();
    std::cout.rdbuf(old);

    std::istringstream iss(buf.str());
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            battle_log.push_back(line);
        }
    }
}

// ─────────────────────────── main ───────────────────────────────────

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    httplib::Server svr;

    // ── Serve index.html ────────────────────────────────────────────
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::ifstream f("web/index.html");
        if (f.is_open()) {
            std::string content((std::istreambuf_iterator<char>(f)),
                                 std::istreambuf_iterator<char>());
            res.set_content(content, "text/html; charset=utf-8");
        } else {
            res.status = 404;
            res.set_content("index.html not found", "text/plain");
        }
    });

    // ── CORS preflight ───────────────────────────────────────────────
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        add_cors(res);
        res.status = 204;
    });

    // ── GET /api/state ───────────────────────────────────────────────
    svr.Get("/api/state", [](const httplib::Request&, httplib::Response& res) {
        add_cors(res);
        std::lock_guard<std::mutex> lock(g_mutex);
        res.set_content(state_to_json(g_state), "application/json");
    });

    // ── POST /api/start ──────────────────────────────────────────────
    // Body: {"p1_id":1,"p2_id":2}   (1=Charmander 2=Squirtle 3=Bulbasaur 4=Pidgey)
    svr.Post("/api/start", [](const httplib::Request& req, httplib::Response& res) {
        add_cors(res);
        std::lock_guard<std::mutex> lock(g_mutex);

        int p1_id = extract_int(req.body, "p1_id");
        int p2_id = extract_int(req.body, "p2_id");

        if (p1_id < 1 || p1_id > 4 || p2_id < 1 || p2_id > 4 || p1_id == p2_id) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid pokemon selection\"}", "application/json");
            return;
        }

        // Reset state
        g_state.p1           = std::make_unique<Pokemon>(Pokemon::create_from_id(p1_id));
        g_state.p2           = std::make_unique<Pokemon>(Pokemon::create_from_id(p2_id));
        g_state.p1_used_heal = false;
        g_state.p2_used_heal = false;
        g_state.turn         = 0;
        g_state.started      = true;
        g_state.over         = false;
        g_state.winner       = "";
        g_state.battle_log.clear();

        g_state.battle_log.push_back("=== Battle Start! ===");
        g_state.battle_log.push_back("Player 1 chose " + g_state.p1->get_name() + "!");
        g_state.battle_log.push_back("Player 2 chose " + g_state.p2->get_name() + "!");
        g_state.battle_log.push_back("--- Player 1's turn ---");

        res.set_content(state_to_json(g_state), "application/json");
    });

    // ── POST /api/attack ─────────────────────────────────────────────
    // Body: {"move_index":0}
    svr.Post("/api/attack", [](const httplib::Request& req, httplib::Response& res) {
        add_cors(res);
        std::lock_guard<std::mutex> lock(g_mutex);

        if (!g_state.started || g_state.over) {
            res.status = 400;
            res.set_content("{\"error\":\"No active battle\"}", "application/json");
            return;
        }

        int move_index = extract_int(req.body, "move_index");

        Pokemon& attacker = (g_state.turn == 0) ? *g_state.p1 : *g_state.p2;
        Pokemon& defender = (g_state.turn == 0) ? *g_state.p2 : *g_state.p1;

        // Validate move before calling (attack_other has a blocking cin loop on bad input)
        if (move_index < 0 || move_index >= attacker.get_num_moves()) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid move index\"}", "application/json");
            return;
        }
        Move m = attacker.get_move(move_index);
        if (m.uses_left <= 0) {
            res.status = 400;
            res.set_content("{\"error\":\"No uses left for this move\"}", "application/json");
            return;
        }

        capture_cout(g_state.battle_log, [&]() {
            attacker.attack_other(defender, move_index);
        });

        if (defender.is_fainted()) {
            g_state.over   = true;
            g_state.winner = attacker.get_name();
            g_state.battle_log.push_back("=== Battle Over! ===");
            g_state.battle_log.push_back(attacker.get_name() + " Wins!");
        } else {
            g_state.turn = 1 - g_state.turn;
            g_state.battle_log.push_back("--- Player " + std::to_string(g_state.turn + 1) + "'s turn ---");
        }

        res.set_content(state_to_json(g_state), "application/json");
    });

    // ── POST /api/heal ───────────────────────────────────────────────
    // Body: {}
    svr.Post("/api/heal", [](const httplib::Request& req, httplib::Response& res) {
        (void)req;
        add_cors(res);
        std::lock_guard<std::mutex> lock(g_mutex);

        if (!g_state.started || g_state.over) {
            res.status = 400;
            res.set_content("{\"error\":\"No active battle\"}", "application/json");
            return;
        }

        bool&    used_heal = (g_state.turn == 0) ? g_state.p1_used_heal : g_state.p2_used_heal;
        Pokemon& current   = (g_state.turn == 0) ? *g_state.p1 : *g_state.p2;

        if (used_heal) {
            res.status = 400;
            res.set_content("{\"error\":\"Heal already used\"}", "application/json");
            return;
        }

        capture_cout(g_state.battle_log, [&]() {
            current.heal();
        });

        used_heal      = true;
        g_state.turn   = 1 - g_state.turn;
        g_state.battle_log.push_back("--- Player " + std::to_string(g_state.turn + 1) + "'s turn ---");

        res.set_content(state_to_json(g_state), "application/json");
    });

    // ── POST /api/reset ──────────────────────────────────────────────
    svr.Post("/api/reset", [](const httplib::Request& req, httplib::Response& res) {
        (void)req;
        add_cors(res);
        std::lock_guard<std::mutex> lock(g_mutex);
        g_state = GameState{};
        res.set_content("{\"ok\":true}", "application/json");
    });

    std::cout << "========================================" << std::endl;
    std::cout << "  Pokemon Battle Server running at:" << std::endl;
    std::cout << "  http://localhost:8080" << std::endl;
    std::cout << "  Open the URL above in your browser." << std::endl;
    std::cout << "========================================" << std::endl;

    svr.listen("0.0.0.0", 8080);
    return 0;
}
