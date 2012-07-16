#include <vector>

#ifndef __MINE__
#define __MINE__

// Map symbols
#define EMPTY ' '
#define EARTH '.'
#define LAMBDA '\\'
#define OPEN_LIFT 'O'
#define CLOSED_LIFT 'L'
#define ROCK '*'
#define ROBOT 'R'
#define WALL '#'
#define BEARD 'W'
#define RAZOR '!'
// TODO: Special symbols

// Various possible commands
#define NUM_STANDARD_COMMANDS 5
#define EXTRA_COMMANDS 2
extern char standardCommands[NUM_STANDARD_COMMANDS];
extern char extraCommands[EXTRA_COMMANDS];

// Map states
#define ABORT 1
#define LOSE 2
#define WIN 3

// Constants for specials
#define NUM_TRAMPOLINES 9

// Defines a minimal interface for a MineState,
// without consideration of implementation.
// Members that can be most efficently calculated
// using only other pure members are implemented.

class MineState {
public:
	// Array access
	virtual char& operator()(int height, int width) = 0;
	// Printing
	virtual const std::string toString() const = 0;
	friend std::ostream& operator<<(std::ostream& stream, const MineState& obj);

	// Copying
	virtual MineState* copySelf() = 0;

	//Virtual Setters
	virtual void setElement(std::pair<int, int> loc, char value) = 0;
	virtual void setRobot(std::pair<int, int> loc) = 0; // Warning: This will overwrite the element at the robot location
	virtual void setDoneType(int doneType) = 0;
	virtual void setDone(bool done) = 0;
	virtual void setMoves(int moves) = 0;
	virtual void setLambdas(int lambdas) = 0;
	virtual void setStepsUnderwater(int steps) = 0;

	// Incrementers
	virtual void incrementMoves() = 0;
	virtual void incrementLambdas() = 0;
	virtual void incrementStepsUnderwater() = 0;
	virtual void incrementRazors() = 0;
	virtual void decrementRazors() = 0;

	//Virtual Transient Getters
	virtual const char& getElement(std::pair<int, int> loc) const = 0;
	virtual const int& getWidth() const = 0;
	virtual const int& getHeight() const = 0;
	virtual const std::pair<int, int>& getRobot() const = 0;

	virtual bool isSpaceMoveable(std::pair<int, int> loc) = 0;

	// Non-Transient Map Properties
	virtual int getScore() = 0;
	virtual const bool& isDone() const = 0;
	virtual const int& getDoneType() const = 0;
	virtual const int& getMoves() const = 0;
	virtual const int& getLambdas() const = 0;
	virtual const int& getStepsUnderwater() const = 0;

	// Metadata
	virtual const int& getWater() const = 0;
	virtual const int& getFlooding() const = 0;
	virtual const int& getWaterproof() const = 0;
	virtual const std::vector<std::pair<char, char> >& getTrampolines() const = 0;
	virtual const int& getGrowth() const = 0;
	virtual const int& getRazors() const = 0;

	// Specials
	virtual int getWaterLevel() = 0;
	virtual char getTrampolineTarget(char trampoline) = 0;
	virtual bool beardGrows() = 0;
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
	std::vector<std::pair<char, char> > trampolines;
	int growth;
	int razors;

	// State data
	bool done;
	int doneType;
	int moves;
	int lambdas;
	int stepsUnderwater;

public:
	// Array access
	char& operator()(int height, int width);
	// Printing
	const std::string toString() const;

	// Constructors
	NaiveMineState(std::string mineText);
	NaiveMineState(const NaiveMineState& base);
	NaiveMineState(const MineState*& base);

	// Desctructors
	~NaiveMineState();

	// Copying
	MineState* copySelf();

	// Setters
	void setElement(std::pair<int, int> loc, char value);
	void setRobot(std::pair<int, int> loc); // Warning: This will overwrite the element at the robot location
	void setDoneType(int doneType);
	void setDone(bool done);
	void setMoves(int moves);
	void setLambdas(int lambdas);
	void setStepsUnderwater(int steps);

	// Incrementers
	void incrementMoves();
	void incrementLambdas();
	void incrementStepsUnderwater();
	void incrementRazors();
	void decrementRazors();
	
	//***Getters***//

	// Transient Map Properties...
	const char& getElement(std::pair<int, int> loc) const;
	const int& getWidth() const;
	const int& getHeight() const;
	const std::pair<int, int>& getRobot() const;

	bool isSpaceMoveable(std::pair<int, int> loc);

	// Non-Transient Map Properties
	int getScore();
	
	const bool& isDone() const;
	const int& getDoneType() const;
	const int& getMoves() const;
	const int& getLambdas() const;
	const int& getStepsUnderwater() const;

	// Metadata
	const int& getWater() const;
	const int& getFlooding() const;
	const int& getWaterproof() const;
	const std::vector<std::pair<char, char> >& getTrampolines() const;
	const int& getGrowth() const;
	const int& getRazors() const;

	// Specials
	int getWaterLevel();
	char getTrampolineTarget(char trampoline);
	bool beardGrows();
	
};

MineState* stepMineState(MineState* state, char command);

MineState* transduceMineState(MineState* state, std::string commands);
MineState* transduceMineState(MineState* state, std::string commands, bool print);
MineState* transduceMineState(MineState* state, std::string commands, bool print, float delay);

#endif
