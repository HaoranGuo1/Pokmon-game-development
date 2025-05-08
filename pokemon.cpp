#include <iostream>
#include "pokemon.hpp"

void Pokemon::heal(){
    current_hp += 10;
    if (current_hp > max_hp){
        current_hp = max_hp;
    } 
}

Pokemon::Pokemon(std::string name, int current_hp, int num_moves, std::string* move_names, int* move_damages){
    this->name = name;
    this->current_hp = current_hp;
    this-> num_moves = num_moves;

    this-> move_names = new std::string[num_moves];
    this->move_damages = new int[num_moves];
    for(int i = 0; i < num_moves; i++){
        this->move_names[i] = move_names[i];
        this->move_damages[i] = move_damages[i];    
    }
    this->attack = 50;
    this->defense = 30;
}

Pokemon::~Pokemon(){
    delete [] move_names;
    delete [] move_damages;
}

std::string Pokemon::get_name() const {
    return name;
}


void Pokemon::Attack(Pokemon& target, int move_index) {
    if (move_index < 0 || move_index >= num_moves) {
        std::cout << "Invalid move index!" << std::endl;
        return;
    }
   int damage = move_damages[move_index] + (attack / 2) - (target.defense / 4);
    if (damage < 0) damage = 0;

   std::cout << name << " attacks " << target.get_name()
              << " using " << move_names[move_index]
              << " for " << damage << " damage." << std::endl;

    int new_hp = target.get_hp() - damage;
    if (new_hp < 0) new_hp = 0;
    target.set_hp(new_hp, target.max_hp);
}


void Pokemon::print_hp(){
    std::cout << name << "s HP: " << current_hp << std::endl;
}

void Pokemon::set_hp(int current, int max){
    current_hp = current;
    max_hp = max;
}

int Pokemon::get_hp() const {
    return current_hp;
}