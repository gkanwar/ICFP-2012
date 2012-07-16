#ifndef __MINE_SEARCH__
#define __MINE_SEARCH__

#include "mine.h"

namespace mineStateSearch {
    // Just check if the robot is in the right place
    bool equals(MineState* node1, MineState* node2) {
	return (node1->getRobot() == node2->getRobot());
    }

    // Neighbors are all possible commands from the current node
    std::vector< Edge< MineState* > > neighbors(MineState* node) {
	char command;
	MineState* tempMineState;
	std::vector< Edge< MineState* > > edgesOut;
	for (int i = 0; i < NUM_STANDARD_COMMANDS; i++) {
	    command = standardCommands[i];
	    if (i != 'W') {
		tempMineState = stepMineState(node, command);
		if (
		    tempMineState->getRobot() != node->getRobot() &&
		    !(tempMineState->isDone() && tempMineState->getDoneType() == LOSE)
		    )
		{
		    // We only want to add the node if it's not equivalent to waiting
		    edgesOut.push_back(*(new Edge<MineState*>(tempMineState, 1)));
		}
	    }
	    else {
		tempMineState = stepMineState(node, command);
		if (!(tempMineState->isDone() && tempMineState->getDoneType() == LOSE)) {
		    edgesOut.push_back(*(new Edge<MineState*>(tempMineState, 1)));
		}
	    }
	}

	return edgesOut;
    }

    // Use euclidean distance as the heuristic
    float heuristic (MineState* node, MineState* goal) {
	std::pair<int, int> robot = node->getRobot();
	std::pair<int, int> robotGoal = goal->getRobot();
	int xDist = robot.first - robotGoal.first;
	int yDist = robot.second - robotGoal.second;
	return sqrt(xDist*xDist + yDist*yDist);
    }
}

std::vector<MineState*> waypointAStar(MineState* initialState, MineState* goalState) {
    return aStarSearch(initialState, goalState, mineStateSearch::equals, mineStateSearch::neighbors, mineStateSearch::heuristic);
}

#endif
