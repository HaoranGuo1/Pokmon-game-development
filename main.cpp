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

    Pokemon p1("", "", 0, 0, 0, nullptr, 0);
    Pokemon p2("", "", 0, 0, 0, nullptr, 0);

    // Trainer 1
    if (trainer_1_pokemon_choice == 1) {
        Move* moves = new Move[1]{{"Ember", "fire", 5, 15}};
        p1 = Pokemon("Charmander", "fire", 6, 4, 18, moves, 1);
    } else if (trainer_1_pokemon_choice == 2) {
        Move* moves = new Move[2]{
            {"Tackle", "normal", 3, 10000},
            {"Water Gun", "water", 5, 3}
        };
        p1 = Pokemon("Squirtle", "water", 4, 6, 22, moves, 2);
    } else {
        Move* moves = new Move[2]{
            {"Tackle", "normal", 3, 10000},
            {"Vine Whip", "grass", 5, 3}
        };
        p1 = Pokemon("Bulbasaur", "grass", 5, 5, 20, moves, 2);
    }

    // Trainer 2
    if (trainer_2_pokemon_choice == 1) {
        Move* moves = new Move[1]{{"Ember", "fire", 5, 15}};
        p2 = Pokemon("Charmander", "fire", 6, 4, 18, moves, 1);
    } else if (trainer_2_pokemon_choice == 2) {
        Move* moves = new Move[2]{
            {"Tackle", "normal", 3, 10000},
            {"Water Gun", "water", 5, 3}
        };
        p2 = Pokemon("Squirtle", "water", 4, 6, 22, moves, 2);
    } else {
        Move* moves = new Move[2]{
            {"Tackle", "normal", 3, 10000},
            {"Vine Whip", "grass", 5, 3}
        };
        p2 = Pokemon("Bulbasaur", "grass", 5, 5, 20, moves, 2);
    }

	// TODO Create a battle object containing the two pokemon objects
	// created above, and then start the battle

    Battle battle(p1, p2);
    battle.run();

    return 0;
}
