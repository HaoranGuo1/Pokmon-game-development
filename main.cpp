#include <iostream>
#include "pokemon.hpp"
#include "battle.hpp"
#include "move.hpp"
#include "startup.hpp"

int main() {
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

    Pokemon p1 = Pokemon::create_from_id(trainer_1_pokemon_choice);
    Pokemon p2 = Pokemon::create_from_id(trainer_2_pokemon_choice);

	// TODO Create a battle object containing the two pokemon objects
	// created above, and then start the battle

    Battle battle(p1, p2);
    battle.run();

    return 0;
}
