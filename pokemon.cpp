#include <iostream>
#include <cmath>    // for ceil() 
#include <cstdlib>  // for rand()
#include "pokemon.hpp"

/*
    Author: Haoran Guo
    Description: Implements the Pokemon class, including dynamic memory management and battle-related behavior like attacking and healing.
*/

// Constructor
Pokemon::Pokemon(std::string name, std::string type, int attack, int defense, int max_hp, Move* moves, int num_moves) {
    this->name = name;
    this->type = type;
    this->attack = attack;
    this->defense = defense;
    this->max_hp = max_hp;
    this->hp = max_hp;

    this->num_moves = num_moves;
    this->moves = new Move[num_moves];

    for(int i = 0; i < num_moves; i++){
        this->moves[i] = moves[i];
    }
}

// Copy constructor
Pokemon::Pokemon(const Pokemon& other) {
    name = other.name;
    type = other.type;
    attack = other.attack;
    defense = other.defense;
    max_hp = other.max_hp;
    hp = other.hp;
    num_moves = other.num_moves;

    moves = new Move[num_moves];
    for (int i = 0; i < num_moves; i++) {
        moves[i] = other.moves[i];
    } 
}

// Assignment operator
Pokemon& Pokemon::operator=(const Pokemon& other) {
    if (this != &other) {
        delete[] moves;

        name = other.name;
        type = other.type;
        attack = other.attack;
        defense = other.defense;
        max_hp = other.max_hp;
        hp = other.hp;
        num_moves = other.num_moves;

        moves = new Move[num_moves];
        for (int i = 0; i < num_moves; i++) {
            moves[i] = other.moves[i];
        } 
    }
    return *this;
}

// Destructor
Pokemon::~Pokemon() {
    delete[] moves;
}

/*
    Function: attack_other
    Description: This function allows the current Pokémon to attack another Pokémon using a selected move. It calculates damage based on type and stats, applies critical hit chance, and reduces the opponent's HP.
    Parameters:
        - target (Pokemon&): the Pokémon to be attacked
        - move_index (int): index of the move to be used
    Side effects: modifies target.hp and prints attack information
*/
void Pokemon::attack_other(Pokemon& target, int move_index) {
    while (true) {
        if (move_index < 0 || move_index >= num_moves) {
            std::cout << "Invalid move index!" << std::endl;
        } else if (moves[move_index].uses_left <= 0){
            std::cout << "No uses left for this move!" << std::endl;
        } else {
            break;
        }

        // Reselected
        print_moves();
        std::cout << "Please select a valid move: ";
        std::cin >> move_index;
    }

    Move move = moves[move_index];

    // Type multiplier logic
    double multiplier = 1.0;
    if (move.type == "fire") {
        if (target.type == "grass") multiplier = 2.0;
        else if (target.type == "water") multiplier = 0.5;
    } else if (move.type == "water") {
        if (target.type == "fire") multiplier = 2.0;
        else if (target.type == "grass") multiplier = 0.5;
    } else if (move.type == "grass") {
        if (target.type == "water") multiplier = 2.0;
        else if (target.type == "fire") multiplier = 0.5;
        else if (target.type == "flying") multiplier = 0.5;
    } else if (move.type == "flying") {
        if (target.type == "grass") multiplier = 2.0;
    }

    // Critical hit (10% chance)
    int critical = (rand() % 10 == 0) ? 3 : 1;

    // Calculate damage using formula
    double base = static_cast<double>(attack) / target.defense;
    double raw_damage = base * multiplier * move.base_damage * critical;
    int damage = static_cast<int>(std::ceil(raw_damage));

    // Apply damage
    target.hp -= damage;
    if (target.hp < 0) {
        target.hp = 0;
    }

    // Use up one move
    moves[move_index].uses_left--;

    // Print result
    std::cout << name << " uses " << move.name << " on " << target.name << " for " << damage << " damage!" << std::endl;

    std::cout << "Value of type_interaction: " << multiplier << std::endl;
    std::cout << "Value of random_critical: " << critical << std::endl;
    std::cout << "Value of damage: " << damage << std::endl;

    if (critical == 3) {
        std::cout << "It was a critical hit!" << std::endl;
    }

    if (multiplier == 2.0) {
        std::cout << "It's super effective!" << std::endl;
    } else if (multiplier == 0.5) {
        std::cout << "It's not very effective..." << std::endl;
    } 
}

/*
    Function: heal
    Description: Restores 10 HP to the Pokémon, without going over max HP.
 */
void Pokemon::heal() {
    hp += 10;
    if (hp > max_hp){
        hp = max_hp;
    } 
    std::cout << name << "healed! Current HP: " << hp << "/" << max_hp << std::endl;
}

/*
    Function: print_stats
    Description: Displays the Pokémon's name and current HP.
*/
void Pokemon::print_status() const {
    std::cout << name << " - HP: " << hp << "/" << max_hp << std::endl;
}

/*
    Function: print_moves
    Description: Prints all available moves and their remaining uses.
*/
void Pokemon::print_moves() const {
    for (int i = 0; i < num_moves; i++) {
        std::cout << i << ": " << moves[i].name 
            << " (" << moves[i].type << ", " 
            << moves[i].base_damage << " damage, uses remaining: " 
            << moves[i].uses_left << ")" << std::endl;
    }
}

/*
    Function: is_fainted
    Description: Returns true if the Pokémon's HP is 0.
 */
bool Pokemon::is_fainted() const {
    return hp <= 0;
}

/*
    Function: get_name
    Description: Returns the Pokémon's name.
 */
std::string Pokemon::get_name() const {
    return name;
}


/*
    Function: get_num_moves
    Description: Returns the number of available moves.
 */
int Pokemon::get_num_moves() const {
    return num_moves;
}

/*
    Function: get_move
    Description: Returns a copy of the move at the given index.
 */
Move Pokemon::get_move(int index) const {
    if (index >= 0 && index < num_moves) {
        return moves[index];
    }
    // Return a dummy move if out of range
    Move dummy;
    dummy.name = "Invalid";
    dummy.type = "none";
    dummy.base_damage = 0;
    dummy.uses_left = 0;
    return dummy;
}

/*
    Function: use_move
    Description: Decreases the usage count of the selected move by 1.
 */
void Pokemon::use_move(int index) {
    if (index >= 0 && index < num_moves && moves[index].uses_left > 0) {
        moves[index].uses_left--;
    }
}

/*
    Data Table: POKEMON_TABLE
    Description: A constant array containing the base data for 4 predefined Pokémon.
    Each entry contains name, type, stats, number of moves, and an array of move data.
    This table is used by create_from_id() to instantiate Pokémon objects.
*/
const PokemonData POKEMON_TABLE[] = {
    { "Charmander", "fire", 6, 4, 18, 1,
      { { "Ember", "fire", 5, 15 }, { "", "", 0, 0 } }
    },
    { "Squirtle", "water", 4, 6, 22, 2,
      { { "Tackle", "normal", 3, 10000 }, { "Water Gun", "water", 5, 3 } }
    },
    { "Bulbasaur", "grass", 5, 5, 20, 2,
      { { "Tackle", "normal", 3, 10000 }, { "Vine Whip", "grass", 5, 3 } }
    },
    { "Pidgey", "flying", 4, 4, 18, 2,
      { { "Tackle", "normal", 3, 10000 }, { "Wing Attack", "flying", 5, 3 } }
    }
};

/*
    Function: create_from_id
    Description: Constructs a Pokemon object based on its ID in the static POKEMON_TABLE.
    The function dynamically allocates memory for the Pokémon's moves and copies the move data.
    Parameters:
        - id (int): The ID of the Pokémon to create (1-based index: 1 = Charmander, etc.)
    Returns:
        - A new Pokemon object initialized with the corresponding data from POKEMON_TABLE.
    Side effects:
        - Dynamically allocates memory for the moves array. Caller must manage object lifetime.
*/
Pokemon Pokemon::create_from_id(int id) {
    const PokemonData& data = POKEMON_TABLE[id - 1];
    // Allocate memory for move array
    Move* moves = new Move[data.num_moves];
    for (int i = 0; i < data.num_moves; ++i) {
        moves[i].name = data.moves[i].name;
        moves[i].type = data.moves[i].type;
        moves[i].base_damage = data.moves[i].base_damage;
        moves[i].uses_left = data.moves[i].uses_left;
    }
    // Create and rerurn Pokemon object
    return Pokemon(data.name, data.type, data.attack, data.defense, data.hp, moves, data.num_moves);
}
