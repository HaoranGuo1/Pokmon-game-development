#include <iostream>

#include "startup.hpp"
#include "pokemon.hpp"
#include "battle.hpp"

int main() {

	Battle b;
	// Pokemon p;
	// p.set_hp(45,50);

	// std::cout << "Before heal: " << p.get_hp() << std::endl;

	// p.heal();

	// std::cout << "after heal: " << p.get_hp() << std::endl;
	
 	std::string pikachu_names[] = {"Thunderbolt", "Quick Attack"};
    int pikachu_damages[] = {40, 20};

    std::string bulbasaur_names[] = {"Vine Whip", "Tackle"};
    int bulbasaur_damages[] = {30, 15};

    Pokemon pikachu("Pikachu", 100, 2, pikachu_names, pikachu_damages);
    Pokemon bulbasaur("Bulbasaur", 100, 2, bulbasaur_names, bulbasaur_damages);

    bulbasaur.print_hp();
    pikachu.Attack(bulbasaur, 0);  // Thunderbolt
    bulbasaur.print_hp();

    return 0;



	// You can edit this starter code however you'd like. All it does is
	// ask the two trainers which pokemon they'd like to play with,
	// returning integers to represent their choices (1 for charmander,
	// 2 for squirtle, 3 for bulbasaur)
	int trainer_1_pokemon_choice = prompt_first_pokemon();
	int trainer_2_pokemon_choice = prompt_second_pokemon(
		trainer_1_pokemon_choice
		
);

	// TODO Use if statements to determine which pokemon was chosen by
	// each player and create respective pokemon objects with the
	// appropriate types, stats, and moves

	// TODO Create a battle object containing the two pokemon objects
	// created above, and then start the battle
}
