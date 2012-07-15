/*
 * mine.h
 */

#ifndef MINE_H_INCLUDED
#define MINE_H_INCLUDED

#include <string>

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

// This is a map representation that naively makes copies
// of the Whole Damn Map on each state update.
class NaiveMineState
{
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
    // Constructors
    NaiveMineState(std::string mineText);
    NaiveMineState(const NaiveMineState& base);
    // Array access
    char& operator()(int height, int width);
    // Getters and setters
    const char& getElement(std::pair<int, int> loc) const;
    void setElement(std::pair<int, int> loc, char value);
    const std::pair<int, int>& getRobot() const;
    void setRobot(std::pair<int, int> loc); // Warning: This will overwrite the element at the robot location
    const int& getWidth() const;
    const int& getHeight() const;
    const int& getDoneType() const;
    void setDoneType(int doneType);
    const bool& isDone() const;
    void setDone(bool done);
    const int& getMoves() const;
    void setMoves(int moves);
    const int& getLambdas() const;
    void setLambdas(int lambdas);
    // TODO: Add meta data getters

    // Calculate the score based on state
    int getScore();
};

// Mine state functions
template<class MineStateType>
MineStateType stepMineState(MineStateType state, char command);
template<class MineStateType>
MineStateType transduceMineState(MineStateType state, char* commands, int numCommands);
template<class MineStateType>
void printMineState(MineStateType state);

template<class MineStateType> class MineSimulator
{
private:
    MineStateType mineState;

public:
    // Constructor
    MineSimulator(std::string mineText);
    // Printing
    void printState();
    // Control the simulator
    void step(char command);
    void transduce(char* commands, int numCommands);
};
	
#endif // MINE_H_INCLUDED
