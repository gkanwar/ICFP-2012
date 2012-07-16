#include <iostream>
#include <cmath>

#include "search.h"
#include "dijkstraSearch.h"
#include "aStarSearch.h"
#include "mine.h"

template <class T>
void print( std::vector< T > v ) {
	for(
		typename std::vector< T >::iterator itr = v.begin();
		itr != v.end();
                ++itr
	)
	{
	    std::cout << *itr << ", ";
	}
	std::cout<<"\n";
};

/*
namespace intSearch {
	bool equals( int node1, int node2 ) {
		return node1 == node2;
	}

	std::vector< Edge< int > > neighbors( int node ) {
		std::vector< Edge< int > > frontier;

		frontier.push_back( Edge< int >( node + 1, 1 ) );
		frontier.push_back( Edge< int >( node - 1, 1 ) );

		return 	frontier;
	}

	float heuristic( int node, int goal ) {
		return std::abs( node - goal );
	}
}
*/

/*
obstacles = set()
for i in range( -10, 100 ):
	obstacles.add( ( i, 5 ) )
	obstacles.add( ( -10, i ) )

def neighbors( point ):
	(x,y) = point

	candidates = [
		(x-1, y),
		(x+1, y),
		(x, y-1),
		(x, y+1)
	]

	for candidate in candidates:
		if candidate in obstacles:
			candidates.remove( canidate )

	return set( [ (point, 1) for point in candidates] )

def manhattanDistance( point ):
	(x,y) = point
	return abs(x) + abs(y)

def isGoal( point ):
	(x,y) = point
	return ( x == 0 ) and ( y == 0 )

print dynamicSearch( (20,20), isGoal, neighbors, manhattanDistance )
*/
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

int main ()
{
/*
	//Use A* to do a trivial search.
	try {
		print< int >( aStarSearch( 30, 0, intSearch::equals, intSearch::neighbors, intSearch::heuristic ) );
	} catch( int error ) {
		std::cout<< "A* int search example failed.\n";
	}

	//Use Dijkstra's Algorithm to do a trivial search.
	try {
		print< int >( dijkstraSearch( 30, 0, intSearch::equals, intSearch::neighbors ) );
	} catch( int error ) {
		std::cout<< "Dijkstra's int search example failed.\n";
	}
*/

    // Use A* to do a search from one point to another
    MineState* initialMine = new NaiveMineState("\
######\n\
#...R#\n\
#**..#\n\
#. .L#\n\
##\\\\##\n\
#....#\n\
#\\\\*.#\n\
#..\\.#\n\
######");
    MineState* goalMine = new NaiveMineState("\
######\n\
#... #\n\
#**..#\n\
#. .L#\n\
##\\\\##\n\
#....#\n\
#\\\\*.#\n\
#R.\\.#\n\
######");

    try {
	std::vector<MineState*> nodes = aStarSearch(initialMine, goalMine, mineStateSearch::equals, mineStateSearch::neighbors, mineStateSearch::heuristic);
	std::cout << "aStarSearch success" << std::endl;
	for (int i = 0; i < nodes.size(); i++) {
	    MineState* node = nodes[i];
	    std::cout << (*node) << std::endl;
	}
    }
    catch (int error) {
	std::cout << "aStarSearch failed: " << error << std::endl;
    }

    return 0;
}
