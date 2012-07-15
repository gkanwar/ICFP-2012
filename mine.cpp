#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <utility>
#include <vector>
#include <iterator>

#include "mine.h"

// Array access (using parens, for convenience)
char& NaiveMineState::operator()(int height, int width) {
	return grid[height][width];
}

// Constructors
NaiveMineState::NaiveMineState( std::string ascii_mine ) {
	// Initialize variables
	done = false;
	doneType = -1;
	moves = 0;
	lambdas = 0;

	// Interpret the input as a grid
	std::stringstream ss(ascii_mine);
	std::vector<std::string> lines;
	int maxLen;
	while (!ss.eof()) {
		std::string temp;
		getline(ss, temp);
		int size = temp.size();
		// We're at the end of the map
		if (size == 0) {
			break;
		}
		else if (size > maxLen) {
			maxLen = size;
		}
		lines.push_back(temp);
	}

	width = maxLen;
	height = lines.size();

	// Assign memory for grid
	grid = new char*[height];
	for (int i = 0; i < height; i++) {
		grid[i] = new char[width];
		strcpy(grid[i], lines[i].c_str());
		int lineEnd = lines[i].size();
		// Loop through the width
		for (int j = 0; j < width; j++) {
			// Pad with spaces
			if (j >= lineEnd) {
				grid[i][j] = ' ';
			}
			// ... or look for the robot
			else if (grid[i][j] == 'R') {
				robot.first = i;
				robot.second = j;
			}
		}
	}

	// Read in the special conditions
	while (!ss.eof()) {
		std::string temp;
		getline(ss, temp);
		std::stringstream tokenizer(temp);
		std::string token;
		int value;
		tokenizer >> token >> value;
	}
}

NaiveMineState::NaiveMineState(const NaiveMineState& base) {
	// Initialize the variables
	this->height = base.getHeight();
	this->width = base.getWidth();
	this->robot = base.getRobot();
	this->done = base.isDone();
	this->doneType = base.getDoneType();
	this->moves = base.getMoves();
	this->lambdas = base.getLambdas();

	// Initialze the grid
	this->grid = new char*[height];
	for (int i = 0; i < height; i++) {
		this->grid[i] = new char[width];
		for (int j = 0; j < width; j++) {
			this->grid[i][j] = base.getElement(std::pair<int, int>(i, j));
		}
	}

	// Initialize the meta variables (TODO)
}

NaiveMineState::NaiveMineState(MineState*& base) {
	// Initialize the variables
	this->height = base->getHeight();
	this->width = base->getWidth();
	this->robot = base->getRobot();
	this->done = base->isDone();
	this->doneType = base->getDoneType();
	this->moves = base->getMoves();
	this->lambdas = base->getLambdas();

	// Initialize the grid
	this->grid = new char*[height];
	for (int i = 0; i < height; i++) {
		this->grid[i] = new char[width];
		for (int j = 0; j < width; j++) {
			this->grid[i][j] = base->getElement(std::pair<int, int>(i, j));
		}
	}

	// Initialize the meta variables (TODO)
}

// Destructors
NaiveMineState::~NaiveMineState() {
    // Deallocate the grid memory, everything else handles itself
    for (int i = 0; i < height; i++)
    {
	delete[] grid[i];
    }
    delete[] grid;
}

// Getter and setter functions
const char& NaiveMineState::getElement(std::pair<int, int> loc) const {
	return grid[loc.first][loc.second];
}

void NaiveMineState::setElement(std::pair<int, int> loc, char value) {
	grid[loc.first][loc.second] = value;
}

const std::pair<int, int>& NaiveMineState::getRobot() const {
	return robot;
}

void NaiveMineState::setRobot(std::pair<int, int> loc) {
	robot = loc;
}

const int& NaiveMineState::getWidth() const {
	return width;
}

const int& NaiveMineState::getHeight() const {
	return height;
}

const int& NaiveMineState::getDoneType() const {
	return doneType;
}

void NaiveMineState::setDoneType(int doneType) {
	this->doneType = doneType;
}

const bool& NaiveMineState::isDone() const {
	return done;
}

void NaiveMineState::setDone(bool done) {
	this->done = done;
}

const int& NaiveMineState::getMoves() const {
	return moves;
}

void NaiveMineState::setMoves(int moves) {
	this->moves = moves;
}

const int& NaiveMineState::getLambdas() const {
	return lambdas;
}

void NaiveMineState::setLambdas(int lambdas) {
	this->lambdas++;
}

int NaiveMineState::getScore() {
	// Gets the current score
	// NOTE: This is only the final score is isDone() returns true
	int score = lambdas*25 - moves;
	if (!done || done && doneType == ABORT) {
		score += lambdas*25;
	}
	else if (doneType == WIN) {
		score += lambdas*50;
	}
}

MineState* stepMineState(MineState* state, char command) {
	// Copy the old state
	MineState* newState = new NaiveMineState( state );

	// Set some flags
	bool moved = false;
	bool abort = false;

	// Robot movement
	std::pair<int, int> robotNew = state->getRobot();
	if (command == 'L') {
		robotNew.second--;
	}
	else if (command == 'R') {
		robotNew.second++;
	}
	else if (command == 'U') {
		robotNew.first++;
	}
	else if (command == 'D') {
		robotNew.first--;
	}
	
	char newLocObject = (*state)(robotNew.first, robotNew.second);

	// Check target location
	switch(newLocObject) {
		case EMPTY:
		case EARTH:
		case LAMBDA:
		case OPEN_LIFT:
		{
			newState->setRobot(robotNew);
			if (newLocObject == LAMBDA) {
				newState->setLambdas(newState->getLambdas()+1);
			}
			else if (newLocObject == OPEN_LIFT) {
				newState->setDone(true);
				newState->setDoneType(WIN);
			}
			break;
		}
		case ROCK:
		{
			// WARNING: We should probably check bounds here for safety
			// (even though maps are wall surrounded)
			if (command == 'R' && (*state)(robotNew.first, robotNew.second+1) == EMPTY) {
				(*newState)(robotNew.first, robotNew.second+1) = ROCK;
				newState->setRobot(robotNew);
			}
			else if (command == 'L' && (*state)(robotNew.first, robotNew.second-1) == EMPTY) {
				(*newState)(robotNew.first, robotNew.second-1) = ROCK;
				newState->setRobot(robotNew);
			}
			else {
				moved = false;
			}
			break;
		}
		default:
		{
			moved = false;
			break;
		}
	}

	// Check for the abort command, otherwise update moves
	if (command == 'A') {
		newState->setDone(true);
		newState->setDoneType(ABORT);
	}
	else {
		newState->setMoves(newState->getMoves()+1);
	}

	// Update the map state
	bool sawClosedLift = false;
	std::pair<int, int> closedLiftCoords;
	bool sawLambda = false;
	for (int i = 0; i < state->getHeight(); i++) {
		for (int j = 0; i < state->getWidth(); j++) {
			if ((*state)(i, j) == ROCK) {
				if (i-1 >= 0 && (*state)(i-1, j) == EMPTY) {
					// Rock falls
					(*newState)(i, j) = EMPTY;
					(*newState)(i-1, j) = ROCK;
				}
				else if (i-1 >= 0 && (*state)(i-1, j) == ROCK) {
					if ((*state)(i, j+1) == EMPTY && (*state)(i-1, j+1) == EMPTY)
					{
						// Rock rolls right
						(*newState)(i, j) = EMPTY;
						(*newState)(i-1, j+1) = ROCK;
					}
					else if ((*state)(i, j-1) == EMPTY && (*state)(i-1, j-1) == EMPTY) {
						// Rock rolls left
						(*newState)(i, j) = EMPTY;
						(*newState)(i-1, j-1) = ROCK;
					}
				}
				else if (i-1 >= 0 && (*state)(i-1, j) == LAMBDA) {
					if (j+1 < (*state).getWidth() && (*state)(i, j+1) == EMPTY && (*state)(i-1, j+1) == EMPTY)
					{
					// Rock rolls right
					(*newState)(i, j) = EMPTY;
					(*newState)(i-1, j+1) = ROCK;
					}
				}
			}
			else if ((*state)(i, j) == LAMBDA) {
				sawLambda = true;
			}
			else if ((*state)(i, j) == CLOSED_LIFT) {
				sawClosedLift = true;
				closedLiftCoords = std::pair<int, int>(i, j);
			}
		}
	}

	// Check if lift opens
	if (sawClosedLift && !sawLambda) {
		(*newState)(closedLiftCoords.first, closedLiftCoords.second) = OPEN_LIFT;
	}

	// TODO: Water

	// Ending conditions
	robotNew = newState->getRobot();
	if(
		robotNew.first+1 < newState->getHeight()
		&& (*newState)(robotNew.first+1, robotNew.second) == ROCK
		&& (*state)(robotNew.first+1, robotNew.second) != ROCK
	) {
		newState->setDone(true);
		newState->setDoneType(LOSE);
	}
	// TODO: Special end conditions, ex. water
	
	return newState;
}

MineState* transduceMine( MineState* state, char* commands, int numCommands ) {
    MineState* newState;

    for (int i = 0; i < numCommands; i++) {
	newState = stepMineState( state, commands[i] );
	delete state;
	state = newState;
    }
    return state;
}

void printMineState( MineState* state ) {
    // Print from bottom to top (since 0,0 is in the bottom left)
    for (int i = state->getHeight()-1; i >= 0; i--) {
	for (int j = 0; j < state->getWidth(); j++) {
	    std::cout << (*state)(i, j);
	}
	std::cout << std::endl;
    }
}
