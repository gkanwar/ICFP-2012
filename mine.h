/*
 * mine.h
 */

#ifndef MINE_H_INCLUDED
#define MINE_H_INCLUDED

#include <string>

// Various possible commands
#define NUM_STANDARD_COMMANDS 5
#define EXTRA_COMMANDS 2
char standardCommands[NUM_STANDARD_COMMANDS] = {'U', 'D', 'L', 'R', 'W'};
char extraCommands[EXTRA_COMMANDS] = {'A', 'S'};

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
    bool winState;
    bool doneState;

public:
    // Constructor
    NaiveMineState(std::string mineText);
    // Array access
    char& operator()(int height, int width);
    // Getters and setters
    char getElement(std::pair<int, int> loc);
    char setElement(std::pair<int, int> loc, char value);
    std::pair<int, int> getRobot();
    void setRobot(std::pair<int, int> loc); // Warning: This will overwrite the element at the robot location
    int getWidth();
    int getHeight();
    bool isWon();
    void setWon(bool won);
    bool isDone();
    void setDone(bool done);
    // TODO: Add meta data getters
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
    void printState(MineStateType state);
    // Control the simulator
    void step(char command);
    void transduce(char* commands, int numCommands);
};
	
#endif // MINE_H_INCLUDED
