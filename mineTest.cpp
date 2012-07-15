#include <iostream>

#include "mine.h"

int main() {
	std::stringstream buffer;
	buffer << std::cin.rdbuf();
	std::string input(buffer.str());
	MineState* initialMine = new NaiveMineState(input);
	printMineState(initialMine);
	return 0;
}
