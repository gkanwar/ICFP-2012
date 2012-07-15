#include <iostream>

#include "mine.h"

int main() {
	std::stringstream buffer;
	buffer << std::cin.rdbuf();
	std::string input(buffer.str());
	NaiveMineState initialMine(input);
	printMineState<NaiveMineState>(initialMine);
	return 0;
}
