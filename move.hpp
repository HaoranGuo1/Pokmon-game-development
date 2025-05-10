#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>

/*
    Author: Haoran Guo
    Description: Defines the move struct, which represents a Pokémon move. Each move has a name, type, base damage, and number of remaining uses.
*/

struct Move {
    std::string name;   // Name of the move (e.g., "Ember")
    std::string type;   // Type of the move (e.g., "fire", "water", etc.)
    int base_damage;    // Base damage value for this move
    int uses_left;      // How many times this move can still be used
};

#endif