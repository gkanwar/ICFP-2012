#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <utility>
#include <vector>
#include <iterator>

#include "mine.h"

// Various possible commands
char standardCommands[NUM_STANDARD_COMMANDS] = {'U', 'D', 'L', 'R', 'W'};
char extraCommands[EXTRA_COMMANDS] = {'A', 'S'};


// Array access (using parens, for convenience)
char& NaiveMineState::operator()(int height, int width) {
	return grid[this->height-height-1][width];
}
// Printing
std::ostream& operator<<(std::ostream& stream, const MineState& obj) {
    stream << obj.toString() << std::endl;

    return stream;
}
const std::string NaiveMineState::toString() const {
    std::stringstream stream;
    stream << "Dimensions: " << this->height << ", " << this->width << std::endl;
    for (int i = 0; i < this->height; i++) {
	for (int j = 0; j < this->width; j++) {
	    stream << this->grid[i][j];
	}
	stream << std::endl;
    }

    return stream.str();
}

// Constructors
NaiveMineState::NaiveMineState(std::string mineText) {
	// Initialize variables
	done = false;
	doneType = -1;
	moves = 0;
	lambdas = 0;
	stepsUnderwater = 0;

	// Interpret the input as a grid
	std::stringstream ss(mineText);
	std::vector<std::string> lines;
	int maxLen = 0;
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
				robot.first = height-i-1;
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
	this->stepsUnderwater = base.getStepsUnderwater();

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

NaiveMineState::NaiveMineState(const MineState*& base) {
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

// Copying
MineState* NaiveMineState::copySelf() {
    return new NaiveMineState((*this));
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
    (*this)(robot.first, robot.second) = EMPTY;
    (*this)(loc.first, loc.second) = ROBOT;
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

void NaiveMineState::incrementMoves() {
	this->moves++;
}

const int& NaiveMineState::getLambdas() const {
	return lambdas;
}

void NaiveMineState::setLambdas(int lambdas) {
	this->lambdas = lambdas;
}

void NaiveMineState::incrementLambdas() {
	this->lambdas++;
}

const int& NaiveMineState::getStepsUnderwater() const {
	return stepsUnderwater;
}

void NaiveMineState::setStepsUnderwater(int steps) {
	this->stepsUnderwater = steps;
}

void NaiveMineState::incrementStepsUnderwater() {
	this->stepsUnderwater++;
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

int NaiveMineState::getWaterLevel() {
	int waterLevel = water;
	if (flooding != 0) {
		waterLevel += moves/flooding;
	}
	return waterLevel;
}

MineState* stepMineState(MineState* state, char command) {
	// Copy the old state
	MineState* newState = state->copySelf();
	std::cout << "Copied old state" << std::endl;

	// Check for done
	if (state->isDone()) {
	    return newState;
	}

	// Set some flags
	bool moved = true;
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
	std::cout << "Got robot movement, newloc: " << robotNew.first << "," << robotNew.second << std::endl;
	
	char newLocObject = (*state)(robotNew.first, robotNew.second);
	std::cout << "Got new location object: (" << newLocObject << ")" << std::endl;

	// Check target location
	switch(newLocObject) {
		case EMPTY:
		case EARTH:
		case LAMBDA:
		case OPEN_LIFT:
		{
			// Update both states, since the robot moves before map is updated
			newState->setRobot(robotNew);		    
			state->setRobot(robotNew);
			if (newLocObject == LAMBDA) {
				newState->incrementLambdas();
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
				(*state)(robotNew.first, robotNew.second+1) = ROCK;
				newState->setRobot(robotNew);
				state->setRobot(robotNew);
			}
			else if (command == 'L' && (*state)(robotNew.first, robotNew.second-1) == EMPTY) {
				(*newState)(robotNew.first, robotNew.second-1) = ROCK;
				(*state)(robotNew.first, robotNew.second-1) = ROCK;
				newState->setRobot(robotNew);
				state->setRobot(robotNew);
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
	std::cout << "Checked target location" << std::endl;

	// Check for the abort command, otherwise update moves
	if (command == 'A') {
		newState->setDone(true);
		newState->setDoneType(ABORT);
	}
	else {
		newState->incrementMoves();
	}
	std::cout << "Updated moves, moved: " << moved << std::endl;

	// Update the map state
	bool sawClosedLift = false;
	std::pair<int, int> closedLiftCoords;
	bool sawLambda = false;
	for (int i = 0; i < state->getHeight(); i++) {
		for (int j = 0; j < state->getWidth(); j++) {
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
	std::cout << "Updated map state" << std::endl;

	// Check if lift opens
	if (sawClosedLift && !sawLambda) {
		(*newState)(closedLiftCoords.first, closedLiftCoords.second) = OPEN_LIFT;
	}

	// Water
	if (newState->getHeight() < newState->getWaterLevel()) {
	  newState->incrementStepsUnderwater();
	}
	else {
	  newState->setStepsUnderwater(0);
	}

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

	if (newState->getStepsUnderwater() > newState->waterproof) {
		newState->setDone(true);
		newState->setDoneType(LOSE);
	}
	
	return newState;
}

MineState* transduceMineState(MineState* state, std::string commands) {
    return transduceMineState(state, commands, false, 0.0f);
}
MineState* transduceMineState(MineState* state, std::string commands, bool print) {
    return transduceMineState(state, commands, print, 0.0f);
}
MineState* transduceMineState(MineState* state, std::string commands, bool print, float delay) {
    MineState* stateCopy = state->copySelf();
    MineState* newState;
    int numCommands = commands.size();

    for (int i = 0; i < numCommands; i++) {
	newState = stepMineState( stateCopy, commands[i] );
	delete stateCopy;
	stateCopy = newState;

	if (print) {
	    std::cout << (*newState) << std::endl;
	    usleep(1000000*delay);
	}
    }
    return newState;
}
