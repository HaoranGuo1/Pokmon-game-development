#include <iostream>
#include "battle.hpp"

/*
    Author: [Your Name]
    Description: Implements the Battle class, which controls a full turn-based Pokémon battle between two players.
 */

Battle::Battle(Pokemon p1, Pokemon p2) : p1(p1), p2(p2), p1_used_heal(false), p2_used_heal(false), turn(0) {}


/*
    Runs the full battle loop until one of the Pokémon faints. Alternates turns between the two player.
*/
void Battle::run() {
    std::cout << "=== Battle Start! ===" << std::endl;

    while(!p1.is_fainted() && !p2.is_fainted()) {
        std::cout << "\nTurn: Player " << (turn + 1) << std::endl;

        if (turn == 0) {
            handle_turn(p1, p2, p1_used_heal);
        } else {
            handle_turn(p2, p1, p2_used_heal);
        }
        turn = 1 - turn;
    }

    std::cout << "\n=== Battle Over! ===" << std::endl;
    if(p1.is_fainted()) {
        std::cout << p2.get_name() << " Wins!" << std::endl;
    } else {
        std::cout << p1.get_name() << " Wins!" << std::endl;
    }
}

/*
    Handles one turn for a player, allowing them to choose to attack or heal. Validates input and updates game state accordingly.
 */
void Battle::handle_turn(Pokemon& attacker, Pokemon& defender, bool& used_heal) {
    attacker.print_status();
    defender.print_status();

    std::cout << "\nWhat would you like to do?" << std::endl;
    std::cout << "1. Attack" << std::endl;
    std::cout << "2. Heal";
    if (used_heal) {
        std::cout << " (Already used)";
    }
    std::cout << std::endl;

    int choice;
    while (true) {
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            attacker.print_moves();
            int move_index;
            std::cout << "Your choice: ";
            std::cin >> move_index;
            attacker.attack_other(defender, move_index);
            break;
        } else if (choice == 2 && !used_heal) {
            attacker.heal();
            used_heal = true;
            break;
        } else {
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    }
}