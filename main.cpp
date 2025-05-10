#include <iostream>

#include "startup.hpp"
#include "pokemon.hpp"
#include "battle.hpp"
#include "move.hpp"

int main() {

	Battle b;
	// Set up moves for Charmander
    Move ember = {"Ember", "fire", 5, 15};
    Move moves1[] = {ember};

    // Set up moves for Bulbasaur
    Move tackle = {"Tackle", "normal", 3, 10000};
    Move vine = {"Vine Whip", "grass", 5, 3};
    Move moves2[] = {tackle, vine};

    // Create two Pokémon
    Pokemon charmander("Charmander", "fire", 6, 4, 18, moves1, 1);
    Pokemon bulbasaur("Bulbasaur", "grass", 5, 5, 20, moves2, 2);

    // Initial status
    std::cout << "Before battle:" << std::endl;
    charmander.print_status();
    bulbasaur.print_status();
    std::cout << std::endl;

    // Charmander attacks Bulbasaur using Ember
    std::cout << "Charmander attacks!" << std::endl;
    charmander.attack_other(bulbasaur, 0);

    // Print status after attack
    std::cout << "\nAfter attack:" << std::endl;
    charmander.print_status();
    bulbasaur.print_status();
    std::cout << std::endl;

    // Bulbasaur heals
    std::cout << "Bulbasaur uses a potion!" << std::endl;
    bulbasaur.heal();

    // Print status after healing
    std::cout << "\nAfter healing:" << std::endl;
    bulbasaur.print_status();

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
