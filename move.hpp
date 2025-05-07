#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>

struct move{
    std::string name;
    std::string type;
    int base_damage;
    int remaining_uses;
};

#endif