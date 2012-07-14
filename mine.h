/*
 * mine.h
 */

class NaiveMineState
{
    // Constructor
    NaiveMineState(string mineText);
    // Getter and setter
    char get(pair<int, int> loc);
    char set(pair<int, int> loc, char value);
    // Move the robot
    char move(pair<int, int> loc);
};

template<class MineStateType>
MineStateType stepMineState(MineStateType state, char command);

template<class MineStateType>
MineStateType transduceMineState(MineStateType state, char* commands, int numCommands);

template<class MineStateType> class MineSimulator
{
    // Constructor
    MineSimulator(string mineText);
    // Printing
    void pprint(MineStateType state);
    // Control the simulator
    void step();
    void transduce();
};
	
