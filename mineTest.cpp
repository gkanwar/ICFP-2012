#include <iostream>
#include <sstream>

#include "mine.h"

int main() {
    // Test reading in a map and printing
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    std::string input(buffer.str());
    MineState* initialMine = new NaiveMineState(input);
    std::cout << initialMine->toString() << std::endl;

    // Initialize with a known map:
    // ######
    // #...R#
    // #**..#
    // #. .L#
    // ##\\##
    // #....#
    // #\\*.#
    // #..\.#
    // ######
    delete initialMine;
    initialMine = new NaiveMineState("\
######\n\
#...R#\n\
#**..#\n\
#. .L#\n\
##\\\\##\n\
#....#\n\
#\\\\*.#\n\
#..\\.#\n\
######");

    // Now perform a command
    MineState* newMine;
    newMine = stepMineState(initialMine, 'D');
    delete initialMine;
    initialMine = newMine;
    std::cout << initialMine->toString() << std::endl;

    // Now perform a series of commands (should in theory solve map)
    newMine = transduceMineState(initialMine, "LDDLRDLLDDRRRUDLLURUUUR", true, 1.0f);
    delete initialMine;
    std::cout << newMine->toString() << std::endl;
    
    return 0;
}
