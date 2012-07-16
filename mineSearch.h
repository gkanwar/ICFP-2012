#ifndef __MINE_SEARCH__
#define __MINE_SEARCH__

#include <cmath>

#include "mine.h"

namespace mineStateSearch {
    // Just check if the robot is in the right place
    bool equals(std::pair< MineState*, char > node1, std::pair< MineState*, char > node2) {
	return (node1.first->getRobot() == node2.first->getRobot());
    }

    // Neighbors are all possible commands from the current node
    std::vector< Edge< std::pair<MineState*, char> > > neighbors(std::pair< MineState*, char > node) {
	char command;
	MineState* tempMineState;
	std::vector< Edge< std::pair<MineState*, char> > > edgesOut;
	for (int i = 0; i < NUM_STANDARD_COMMANDS; i++) {
	    command = standardCommands[i];
	    if (i != 'W') {
		tempMineState = stepMineState(node.first, command);
		if (
		    tempMineState->getRobot() != node.first->getRobot() &&
		    !(tempMineState->isDone() && tempMineState->getDoneType() == LOSE)
		    )
		{
		    // We only want to add the node if it's not equivalent to waiting
		    edgesOut.push_back(*(new Edge< std::pair<MineState*, char> >(std::pair<MineState*, char>(tempMineState, command), 1)));
		}
	    }
	    else {
		tempMineState = stepMineState(node.first, command);
		if (!(tempMineState->isDone() && tempMineState->getDoneType() == LOSE)) {
		    edgesOut.push_back(*(new Edge< std::pair<MineState*, char> >(std::pair<MineState*, char>(tempMineState, command), 1)));
		}
	    }
	}

	return edgesOut;
    }

    // Use euclidean distance as the heuristic
    float heuristic (std::pair< MineState*, char > node, std::pair< MineState*, char > goal) {
	std::pair<int, int> robot = node.first->getRobot();
	std::pair<int, int> robotGoal = goal.first->getRobot();
	int xDist = robot.first - robotGoal.first;
	int yDist = robot.second - robotGoal.second;
	return sqrt(xDist*xDist + yDist*yDist);
    }
}

std::vector< std::pair<MineState*, char> > waypointAStar(MineState* initialState, MineState* goalState) {
    return aStarSearch<std::pair< MineState*, char > >(std::pair< MineState*, char >(initialState, 0), std::pair< MineState*, char >(goalState, 0), mineStateSearch::equals, mineStateSearch::neighbors, mineStateSearch::heuristic);
}

#endif
