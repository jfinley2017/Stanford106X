/*
 * CS 106B/X Twenty Questions
 * This client program contains a text menu for running your game.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2016/10/18
 * - modified for 16au version
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"

// function prototype declarations
extern bool playQuestionGame(std::istream& input);

int main() {
    std::cout << "Welcome to CS 106X 20 Questions!" << std::endl;
    std::cout << "Think of an item, and I will guess it" << std::endl;
    std::cout << "in a total of up to twenty questions." << std::endl;
    std::cout << std::endl;

    std::ifstream input;
    promptUserForFile(input, "Input file name? ");   // open input data file

    int won = 0;
    int lost = 0;
    do {
        // call your recursive play function to play one game
        std::cout << std::endl;
        bool result = playQuestionGame(input);
        if (result) {
            won++;
        } else {
            lost++;
        }
        rewindStream(input);
    } while (getYesOrNo("Play again?"));

    std::cout << std::endl;
    std::cout << "I won " << won << " and lost " << lost << "." << std::endl;
    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}
