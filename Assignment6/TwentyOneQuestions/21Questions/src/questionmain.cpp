/**
 * CS 106B/X 21 Questions
 * This client program contains a text menu for running your game.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2016/11/13
 * - 16au 106x version
 */

#include "questiontree.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"

// default file name where memory of past games is saved between sessions
static const bool USE_MEMORY_FILE = true;
static const std::string MEMORY_FILE = "memory.txt";

int main() {
    std::cout << "Welcome to CS 106X 21 Questions!" << std::endl;
    std::cout << "Think of an item, and I will guess it" << std::endl;
    std::cout << "using a series of yes-or-no questions." << std::endl;
    std::cout << "I can also learn and grow smarter over time." << std::endl;

    QuestionTree tree;

    if (USE_MEMORY_FILE && fileExists(MEMORY_FILE)
            && getYesOrNo("Shall I recall my previous memory state?")) {
        std::ifstream input;
        input.open(MEMORY_FILE.c_str());
        tree.readData(input);
        input.close();
    }

    // menu
    while (true) {
        std::cout << std::endl;
        std::string choice = toUpperCase(trim(getLine("P)lay, R)ead, W)rite, S)tats, Q)uit?")));
        if (choice == "P") {
            tree.playGame();
        } else if (choice == "R") {
            std::ifstream input;
            promptUserForFile(input, "Input file name?");
            std::cout << "Reading question tree ..." << std::endl;
            tree.readData(input);
            input.close();
        } else if (choice == "W") {
            std::ofstream output;
            promptUserForFile(output, "Output file name?");
            std::cout << "Writing question tree ..." << std::endl;
            tree.writeData(output);
            output.close();
        } else if (choice == "S") {
            int won = tree.getGamesWon();
            int lost = tree.getGamesLost();
            int games = won + lost;
            std::cout << "Games won  by me : " << won << std::endl;
            std::cout << "Games lost by me : " << lost << std::endl;
            if (games > 0) {
                double winPct = 100.0 * won / games;
                std::cout << "My win percentage: " << std::fixed
                          << std::setprecision(1) << winPct << std::endl;
            }
        } else if (choice == "Q") {
            break;
        }
    }

    std::cout << std::endl;
    if (USE_MEMORY_FILE) {
        std::cout << "Remembering this memory state for later ..." << std::endl;
        std::ofstream output;
        output.open(MEMORY_FILE.c_str());
        tree.writeData(output);
        output.close();
    }

    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}
