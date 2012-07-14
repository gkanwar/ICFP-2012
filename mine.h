/*
 * mine.h
 */

class NaiveMineState
{
    // Constructor
    NaiveMineState(string mineText);
    // Array access
    char& operator()(int height, int width);
    // Getters and setters
    char getElement(pair<int, int> loc);
    char setElement(pair<int, int> loc, char value);
    pair<int, int> getRobot();
    void setRobot(pair<int, int> loc); // Warning: This will overwrite the element at the robot location
    int getWidth();
    int getHeight();
    // TODO: Add meta data getters
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
	
