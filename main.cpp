#include <iostream>

#include "startup.hpp"
#include "pokemon.hpp"

int main() {
	Pokemon p;
	p.set_hp(45,50);

	std::cout << "Before heal: " << p.get_hp() << std::endl;

	p.heal();

	std::cout << "after heal: " << p.get_hp() << std::endl;
	
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
