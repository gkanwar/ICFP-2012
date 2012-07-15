#ifndef __MINE__
#define __MINE__

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <utility>
#include <vector>
#include <iterator>

// Map symbols
#define EMPTY ' '
#define EARTH '.'
#define LAMBDA '\\'
#define OPEN_LIFT 'O'
#define CLOSED_LIFT 'L'
#define ROCK '*'
#define ROBOT 'R'
#define WALL '#'
// TODO: Special symbols

// Various possible commands
#define NUM_STANDARD_COMMANDS 5
#define EXTRA_COMMANDS 2
char standardCommands[NUM_STANDARD_COMMANDS] = {'U', 'D', 'L', 'R', 'W'};
char extraCommands[EXTRA_COMMANDS] = {'A', 'S'};

// Map states
#define ABORT 1
#define LOSE 2
#define WIN 3

// Defines a minimal interface for a MineState,
// without consideration of implementation.
// Members that can be most efficently calculated
// using only other pure members are implemented.

class MineState {
public:
	// Array access
	virtual char& operator()(int height, int width) = 0;

	//Virtual Setters
	virtual void setElement(std::pair<int, int> loc, char value) = 0;
	virtual void setRobot(std::pair<int, int> loc) = 0; // Warning: This will overwrite the element at the robot location
	virtual void setDoneType(int doneType) = 0;
	virtual void setDone(bool done) = 0;
	virtual void setWon( bool won ) = 0;
	virtual void setMoves(int moves) = 0;
	virtual void setLambdas(int lambdas) = 0;

	//Virtual Transient Getters
	virtual const char& getElement(std::pair<int, int> loc) const = 0;
	virtual const int& getWidth() const = 0;
	virtual const int& getHeight() const = 0;
	virtual const std::pair<int, int>& getRobot() const = 0;

	// Non-Transient Map Properties
	virtual int getScore() = 0;
	virtual const bool& isDone() const = 0;
	virtual const int& getDoneType() const = 0;
	virtual const int& getMoves() const = 0;
	virtual const int& getLambdas() const = 0;
};

// This is a map representation that naively makes copies
// of the Whole Damn Map on each state update.
class NaiveMineState : public MineState {
private:
	// Regular data
	int height, width;
	char** grid;
	std::pair<int, int> robot;

	// Meta data
	int water;
	int flooding;
	int waterproof;
	// TODO: Add in the new meta data

	// State data
	bool done;
	int doneType;
	int moves;
	int lambdas;

public:

	char& operator()(int height, int width);

	// Constructors
	NaiveMineState(std::string mineText);
	NaiveMineState(const NaiveMineState& base);
	NaiveMineState( MineState*& base );

	// Setters
	void setElement(std::pair<int, int> loc, char value);
	void setRobot(std::pair<int, int> loc); // Warning: This will overwrite the element at the robot location
	void setDoneType(int doneType);
	void setDone(bool done);
	void setWon( bool won );
	void setMoves(int moves);
	void setLambdas(int lambdas);
	
	//***Getters***//

	// Transient Map Properties...
	const char& getElement(std::pair<int, int> loc) const;
	const int& getWidth() const;
	const int& getHeight() const;
	const std::pair<int, int>& getRobot() const;

	// Non-Transient Map Properties
	int getScore();
	const bool& isDone() const;
	const int& getDoneType() const;
	const int& getMoves() const;
	const int& getLambdas() const;
	// TODO: Add meta data getters
};

MineState* stepMineState( MineState* state, char command );

MineState* transduceMineState( MineState* state, char* commands, int numCommands );

void printMineState( MineState* state );
	
#include "mine.cpp"
#endif
