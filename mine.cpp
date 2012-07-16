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

	// default metadata
	water = 0;
	flooding = 0;
	waterproof = 10;
	growth = 25;
	razors = 0;

	// Read in the special conditions
	// TODO: actually assign metadata
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

	// Initialize the meta variables
	this->water = base.getWater();
	this->flooding = base.getFlooding();
	this->waterproof = base.getWaterproof();
	this->trampolines = base.getTrampolines();
	this->growth = base.getGrowth();
	this->razors = base.getRazors();
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
	this->stepsUnderwater = base->getStepsUnderwater();

	// Initialize the grid
	this->grid = new char*[height];
	for (int i = 0; i < height; i++) {
		this->grid[i] = new char[width];
		for (int j = 0; j < width; j++) {
			this->grid[i][j] = base->getElement(std::pair<int, int>(i, j));
		}
	}

	// Initialize the meta variables (TODO)
	this->water = base->getWater();
	this->flooding = base->getFlooding();
	this->waterproof = base->getWaterproof();
	this->trampolines = base->getTrampolines();
	this->growth = base->getGrowth();
	this->razors = base->getRazors();

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

bool NaiveMineState::isSpaceMoveable(std::pair<int, int> loc) {
	char targetObject = (*this)(loc.first, loc.second);
	return (targetObject == EMPTY || targetObject == EARTH || targetObject == LAMBDA || targetObject == OPEN_LIFT || targetObject == BEARD);
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

const int& NaiveMineState::getWater() const {
	return water;
}

const int& NaiveMineState::getFlooding() const {
	return flooding;
}

const int& NaiveMineState::getWaterproof() const {
	return waterproof;
}

const std::vector<std::pair<char, char> >& NaiveMineState::getTrampolines() const {
	return trampolines;
}

const int& NaiveMineState::getGrowth() const {
	return growth;
}

const int& NaiveMineState::getRazors() const {
	return razors;
}

void NaiveMineState::incrementRazors() {
	this->razors++;
}

void NaiveMineState::decrementRazors() {
	this->razors--;
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

char NaiveMineState::getTrampolineTarget(char trampoline) {
	for (int i = 0; i < trampolines.size(); i++) {
		if (trampolines[i].first == trampoline) {
			return trampolines[i].second;
		}
	}
	std::cout << "Warning: trampoline not found" << std::endl;
	return -1;
}

bool NaiveMineState::beardGrows() {
	return moves % growth == 0;
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
		case RAZOR:
		{
			// Update both states, since the robot moves before map is updated
			stateCopy->setRobot(robotNew);
			if (newLocObject == LAMBDA) {
				stateCopy->incrementLambdas();
			}
			else if (newLocObject == OPEN_LIFT) {
				stateCopy->setDone(true);
				stateCopy->setDoneType(WIN);
			}
			else if (newLocObject == RAZOR) {
				stateCopy->incrementRazors();
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
		// trampolines
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		{
			char target = state->getTrampolineTarget(newLocObject);
			for (int i = 0; i < state->getHeight(); i++) {
				for (int j = 0; j < state->getWidth(); j++) {
					if ((*state)(i, j) == target) {
						// teleport the robot
						robotNew.first = i;
						robotNew.second = j;
						stateCopy->setRobot(robotNew);
						state->setRobot(robotNew);
					}
					else if (state->getTrampolineTarget((*state)(i, j)) == target) {
						// destroy trampoline
						(*stateCopy)(i, j) = EMPTY;
					}
				}
			}
		}
		default:
		{
			moved = false;
			break;
		}
	}

	// shave
	if (command == 'S' && state->getRazors() > 0) {
		for (int i = robotNew.first-1; i < robotNew.first+1; i++) {
			for (int j = robotNew.second-1; j < robotNew.second+1; j++) {
				if ((*stateCopy)(i, j) == BEARD) {
					(*stateCopy)(i, j) = EMPTY;
				}
			}
		}
		state->decrementRazors();
	}
				

	// Check for the abort command, otherwise update moves
	if (command == 'A') {
		stateCopy->setDone(true);
		stateCopy->setDoneType(ABORT);
	}
	else {
		stateCopy->incrementMoves();
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
			else if ((*stateCopy)(i, j) == BEARD and stateCopy->beardGrows()) {
				for (int ii = i-1; ii <= i+1; ii++) {
					for (int jj = j-1; jj <= j+1; jj++) {
						if ((*stateCopy)(i, j) == EMPTY) {
							(*stateCopy)(i, j) = BEARD;
						}
					}
				}
			}
		}
	}

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
		&& (*stateCopy)(robotNew.first+1, robotNew.second) != ROCK
	) {
		newState->setDone(true);
		newState->setDoneType(LOSE);
	}

	if (newState->getStepsUnderwater() > newState->getWaterproof()) {
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
