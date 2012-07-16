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
    std::cout << "Starting copy" << std::endl << std::flush;
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
    std::cout << "Ending copy" << std::endl << std::flush;
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

bool NaiveMineState::setRobot(std::pair<int, int> loc) {
    char targetObject = (*this)(loc.first, loc.second);
    if (targetObject == EMPTY || targetObject == EARTH || targetObject == LAMBDA || targetObject == OPEN_LIFT)
    {
	(*this)(robot.first, robot.second) = EMPTY;
	(*this)(loc.first, loc.second) = ROBOT;
	robot = loc;
	return true;
    }

    return false;
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
	MineState* stateCopy = state->copySelf();

	// Check for done
	if (state->isDone()) {
	    return stateCopy;
	}

	// Set some flags
	bool moved = true;
	bool abort = false;

	// Robot movement
	std::pair<int, int> robotNew = stateCopy->getRobot();
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
	
	char newLocObject = (*stateCopy)(robotNew.first, robotNew.second);

	// Check target location
	switch(newLocObject) {
		case EMPTY:
		case EARTH:
		case LAMBDA:
		case OPEN_LIFT:
		{
			// Update both states, since the robot moves before map is updated
			stateCopy->setRobot(robotNew);
			if (newLocObject == LAMBDA) {
			    stateCopy->setLambdas(stateCopy->getLambdas()+1);
			}
			else if (newLocObject == OPEN_LIFT) {
			    stateCopy->setDone(true);
			    stateCopy->setDoneType(WIN);
			}
			break;
		}
		case ROCK:
		{
			// WARNING: We should probably check bounds here for safety
			// (even though maps are wall surrounded)
			if (command == 'R' && (*state)(robotNew.first, robotNew.second+1) == EMPTY) {
				(*stateCopy)(robotNew.first, robotNew.second+1) = ROCK;
				stateCopy->setRobot(robotNew);
			}
			else if (command == 'L' && (*state)(robotNew.first, robotNew.second-1) == EMPTY) {
				(*stateCopy)(robotNew.first, robotNew.second-1) = ROCK;
				stateCopy->setRobot(robotNew);
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
		stateCopy->setDone(true);
		stateCopy->setDoneType(ABORT);
	}
	else {
		stateCopy->setMoves(stateCopy->getMoves()+1);
	}

	// Recopy the map to be able to read from old and write to new
	MineState* newState = stateCopy->copySelf();

	// Update the map state
	bool sawClosedLift = false;
	std::pair<int, int> closedLiftCoords;
	bool sawLambda = false;
	for (int i = 0; i < stateCopy->getHeight(); i++) {
		for (int j = 0; j < stateCopy->getWidth(); j++) {
			if ((*stateCopy)(i, j) == ROCK) {
				if (i-1 >= 0 && (*stateCopy)(i-1, j) == EMPTY) {
					// Rock falls
					(*newState)(i, j) = EMPTY;
					(*newState)(i-1, j) = ROCK;
				}
				else if (i-1 >= 0 && (*stateCopy)(i-1, j) == ROCK) {
					if ((*stateCopy)(i, j+1) == EMPTY && (*stateCopy)(i-1, j+1) == EMPTY)
					{
						// Rock rolls right
						(*newState)(i, j) = EMPTY;
						(*newState)(i-1, j+1) = ROCK;
					}
					else if ((*stateCopy)(i, j-1) == EMPTY && (*stateCopy)(i-1, j-1) == EMPTY) {
						// Rock rolls left
						(*newState)(i, j) = EMPTY;
						(*newState)(i-1, j-1) = ROCK;
					}
				}
				else if (i-1 >= 0 && (*stateCopy)(i-1, j) == LAMBDA) {
					if (j+1 < (*stateCopy).getWidth() && (*stateCopy)(i, j+1) == EMPTY && (*stateCopy)(i-1, j+1) == EMPTY)
					{
					// Rock rolls right
					(*newState)(i, j) = EMPTY;
					(*newState)(i-1, j+1) = ROCK;
					}
				}
			}
			else if ((*stateCopy)(i, j) == LAMBDA) {
				sawLambda = true;
			}
			else if ((*stateCopy)(i, j) == CLOSED_LIFT) {
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
		&& (*stateCopy)(robotNew.first+1, robotNew.second) != ROCK
	) {
		newState->setDone(true);
		newState->setDoneType(LOSE);
	}
	// TODO: Special end conditions, ex. water
	
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
