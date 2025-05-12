#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <string>
#include "move.hpp"

/*
    Author: Haoran Guo
    Description: Defines the Pokemon class, which stores a Pokémon's name, type, stats, and a dynamic array of moves. It supports actions like attacking and healing.
*/

struct MoveData {
    const char* name;
    const char* type;
    int base_damage;
    int uses_left;
};

struct PokemonData {
    const char* name;
    const char* type;
    int attack;
    int defense;
    int hp;
    int num_moves;
    MoveData moves[2];
};

class Pokemon {
    private:
        std::string name;
        std::string type;
        int attack, defense, hp, max_hp;

        Move* moves;
        int num_moves;

    public:
        // Constructor
        Pokemon(std::string name, std::string type, int attack, int defense, int max_hp, Move* moves, int num_moves);

        // Copy constructor
        Pokemon(const Pokemon& other);

        // Assignment operator
        Pokemon& operator=(const Pokemon& other);

        // Destructor
        ~Pokemon();

        // perform an attack on another Pokémon
        void attack_other(Pokemon& target, int move_index);

        // Heal this Pokémon
        void heal();

        // Print name and current HP
        void print_status() const;

        // Check if Pokémon is fainted
        bool is_fainted() const;

        // Get Pokémon name
        std::string get_name() const;

        // Get number of moves
        int get_num_moves() const;

        // print move list with usage information
        void print_moves() const;

        // Get move by index (for damage calc)
        Move get_move(int index) const;

        // Reduce move usage by 1
        void use_move(int index);
        
        // id = 1~4
        static Pokemon create_from_id(int id); 
};


#endif