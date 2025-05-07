#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <string>
#include "move.hpp"

class Pokemon {
    private:
        std::string name;
        std::string type;
        move* moves;
        
        int num_moves;
        int attack;
        int defense;
        int current_hp;
        int max_hp;

    public:
        Pokemon();
        ~Pokemon();

        void set_decision(std::string str);
        void heal();
        
        void set_hp(int current, int max);
        int get_hp() const;
};


#endif