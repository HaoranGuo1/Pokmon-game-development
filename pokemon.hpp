#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <string>
#include "move.hpp"

class Pokemon {
    private:
        std::string name;
        std::string type;
        move* moves;
        std::string* move_names;
        int* move_damages;
        
        int num_moves;
        int attack;
        int defense;
        int current_hp;
        int max_hp;

    public:
        Pokemon(std::string name, int current_hp, int num_moves,
            std::string* move_names, int* move_damages);
        ~Pokemon();

        void Attack(Pokemon& target, int move_index);

        void print_hp();

        void set_decision(std::string str);
        void heal();

        std::string get_name() const;

        
        void set_hp(int current, int max);
        int get_hp() const;
};


#endif