#include <iostream>
#include <cstring>
#include <sstream>
#include <utility>
#include <vector>
#include <iterator>

#include "mine.h"

using namespace std;

// Constructors
NaiveMineState::NaiveMineState(string ascii_mine)
{
    // Initialize variables
    winState = false;
    doneState = false;

    // Interpret the input as a grid
    stringstream ss(ascii_mine);
    vector<string> lines = vector<string>();
    int maxLen;
    while (!ss.eof())
    {
	string temp;
	getline(ss, temp);
	int size = temp.size();
	// We're at the end of the map
	if (size == 0)
	{
	    break;
	}
	else if (size > maxLen)
	{
	    maxLen = size;
	}
	lines.push_back(temp);
    }

    width = maxLen;
    height = lines.size();

    // Assign memory for grid
    grid = new char*[height];
    for (int i = 0; i < height; i++)
    {
	grid[i] = new char[width];
	strcpy(grid[i], lines[i].c_str());
	int lineEnd = lines[i].size();
	// Loop through the width
	for (int j = 0; j < width; j++)
	{
	    // Pad with spaces
	    if (j >= lineEnd)
	    {
		grid[i][j] = ' ';
	    }
	    // ... or look for the robot
	    else if (grid[i][j] == 'R')
	    {
		robot.first = i;
		robot.second = j;
	    }
	}
    }

    // Read in the special conditions
    while (!ss.eof())
    {
	string temp;
	getline(ss, temp);
	stringstream tokenizer(temp);
	string token;
	int value;
	tokenizer >> token >> value;
    }
}
NaiveMineState::NaiveMineState(const NaiveMineState& base)
{
    // Initialize the variables
    this->height = base.getHeight();
    this->width = base.getWidth();
    this->robot = base.getRobot();
    this->winState = base.isWon();
    this->doneState = base.isDone();

    // Initialze the grid
    this->grid = new char*[height];
    for (int i = 0; i < height; i++)
    {
	this->grid[i] = new char[width];
	for (int j = 0; j < width; j++)
	{
	    this->grid[i][j] = base.getElement(pair<int, int>(i, j));
	}
    }

    // Initialize the meta variables (TODO)
}

// Array access (using parens, for convenience)
char& NaiveMineState::operator()(int height, int width)
{
    return grid[height][width];
}

// Getter and setter functions
const char& NaiveMineState::getElement(pair<int, int> loc) const
{
    return grid[loc.first][loc.second];
}
void NaiveMineState::setElement(pair<int, int> loc, char value)
{
    grid[loc.first][loc.second] = value;
}
const pair<int, int>& NaiveMineState::getRobot() const
{
    return robot;
}
void NaiveMineState::setRobot(pair<int, int> loc)
{
    robot = loc;
}
const int& NaiveMineState::getWidth() const
{
    return width;
}
const int& NaiveMineState::getHeight() const
{
    return height;
}
const bool& NaiveMineState::isWon() const
{
    return winState;
}
void NaiveMineState::setWon(bool won)
{
    winState = won;
}
const bool& NaiveMineState::isDone() const
{
    return doneState;
}
void NaiveMineState::setDone(bool done)
{
    doneState = done;
}


// Map state functions
template<class MineStateType>
MineStateType stepMineState(MineStateType state, char command)
{
    // Copy the old state
    MineStateType newState(state);
    
    return newState;
}

template<class MineStateType>
MineStateType transduceMineState(MineStateType state, char* commands, int numCommands)
{
    for (int i = 0; i < numCommands; i++)
    {
	state = stepMineState<MineStateType>(state, commands[i]);
    }
    return state;
}


// Constructor
template<class MineStateType>
MineSimulator<MineStateType>::MineSimulator(string mapText)
{
    mineState = MineStateType(mapText);
}
// Printing
template<class MineStateType>
void MineSimulator<MineStateType>::printState()
{
    printMineState<MineStateType>(mineState);
}
// Control the simulator
template<class MineStateType>
void MineSimulator<MineStateType>::step(char command)
{
    mineState = stepMineState<MineStateType>(mineState, command);
}
template<class MineStateType>
void MineSimulator<MineStateType>::transduce(char* commands, int numCommands)
{
    mineState = transduceMineState<MineStateType>(mineState, commands, numCommands);
}

/*
int main()
{
    stringstream buffer;
    buffer << cin.rdbuf();
    string input(buffer.str());
    NaiveMapState initialMap(input);

    int height=1, width=4;
    cout << "Char at loc (" << height << "," << width << "): " << initialMap.get(height, width) << endl;
}
*/
