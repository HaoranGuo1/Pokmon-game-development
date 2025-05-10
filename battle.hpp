#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "pokemon.hpp"

/*
    Author: Haoran Guo
    Description: Defines the Battle class, which controls a full turn-based Pokémon battle between two players.
*/

class Battle {
    private:
        Pokemon p1;         // Player 1's Pokémon
        Pokemon p2;         // Player 2's Pokémon
        bool p1_used_heal;  // Whether Player 1 has used their heal
        bool p2_used_heal;  // Whether Player 2 has used their heal
        int turn;           // 0 = player 1's turn, 1 = player 2's turn

    public:
        // Constructor: Initializes the battle with two Pokémon
        Battle(Pokemon p1, Pokemon p2);

        // Run the full battle loop
        void run();

    private:
        // Handles one player's turn (attack or heal)
        void handle_turn(Pokemon& attacker, Pokemon& defender, bool& used_heal);

};

#endif 
