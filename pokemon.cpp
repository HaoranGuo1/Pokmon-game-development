#include "pokemon.hpp"

void Pokemon::heal(){
    current_hp += 10;
    if (current_hp > max_hp){
        current_hp = max_hp;
    } 
}

Pokemon::Pokemon(){
    moves = nullptr;
}

Pokemon::~Pokemon(){
    if (moves != nullptr){
        delete [] moves;
        moves = nullptr;
    }
}



void Pokemon::set_hp(int current, int max){
    current_hp = current;
    max_hp = max;
}

int Pokemon::get_hp() const {
    return current_hp;
}