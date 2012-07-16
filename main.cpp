#include <iostream>
#include <sstream>

#include "geneticAlgorithm.h"
#include "search.h"
#include "mine.h"
#include "mineSearch.h"

template <class T>
void print( std::vector< T > v ) {
	for(
		typename std::vector< T >::iterator itr = v.begin();
		itr != v.end();
                ++itr
	)
	{
	    std::cout << (*itr).first << " " << (*itr).second << ", ";
	}
	std::cout<<"\n";
};

std::pair< int, int > randomPoint( int height, int width ) {
	return std::pair< int, int >( rand() % height, rand() % width );
}

int manhattanDistance( std::pair<int, int> pointA, std::pair<int, int> pointB ) {
	return std::abs( pointA.first - pointB.first ) + std::abs( pointA.second - pointB.second );
}

class Walk {
private:
	std::vector< std::pair< int, int > > waypoints;
public:
	Walk() {
	}
	Walk( std::pair< int, int > start ) {
		this->waypoints.push_back( start );
	}
	void append( std::pair< int, int > waypoint ) {
		this->waypoints.push_back( waypoint );
	}
	int length() {
		return this->waypoints.size();
	}
	std::pair< int, int >& operator[]( int index ) {
		return this->waypoints[ index ];
	}
	
};

Walk randomWalk( std::pair< int, int > start, int height, int width, int walkLength ) {
	Walk walk( start );
	for( int index = 1; index < walkLength; index ++ ) {
		walk.append( randomPoint( height, width ) );
	}
	return walk;
};

namespace planeSearch {

	bool equals( std::pair<int, int> node1, std::pair<int, int> node2 ) {
		return ( node1.first == node2.first && node1.second == node2.second );
	}

	std::vector< Edge< std::pair<int, int> > > neighbors( std::pair<int, int> node ) {
		std::vector< Edge< std::pair<int, int> > > frontier;

		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first + 1, node.second ), 1 ) );
		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first - 1, node.second ), 1 ) );
		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first, node.second + 1 ), 1 ) );
		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first, node.second - 1 ), 1 ) );
	
		return 	frontier;
	};

	float heuristic( std::pair<int, int> node, std::pair<int, int> goal ) {
		return manhattanDistance( node, goal );
	};
}

namespace walkBreeder {

    MineState* start;
    int width = 30;
    int height = 30;
    int walkLength = 10;

	Walk getRandomCreature() {
	    return randomWalk(start->getRobot(), height, width, walkLength);
	}

	Walk breed(Walk mom, Walk pop) {
	    Walk child(start->getRobot());
		//TODO: randomness!
		for(int index = 1; index < walkLength; index++) {
			std::pair< int, int > waypoint;
			if( rand() % 2 ) {
				waypoint = mom[ index ];
			}
			else {
				waypoint = pop[ index ];
			}

			//Sprinkle with randomness!
			// With chance 1 / 50 ....
			if( rand() % 50 == 0 ) {
				waypoint.first += ( rand() % 3 - 1 );
				if (waypoint.first < 0) { waypoint.first = 0; }
				else if (waypoint.first >= height) { waypoint.first = height-1; }
				waypoint.second += ( rand() % 3 - 1 );
				if (waypoint.second < 0) { waypoint.second = 0; }
				else if (waypoint.second >= width) { waypoint.second = width-1; }
			};
			child.append( waypoint );
		}
		return child;
	}

	float fitness( Walk bob ) {
		float fitness = 0;
		MineState* state = start->copySelf();
		//std::cout << "Checking fitness" << std::endl;
		//std::cout << "path length: " << bob.length() << std::endl;
		MineState* goal;
		bool ret;
		std::vector<MineState*> nodes;
		// For each segment of the path...
		for( int i = 0; i < bob.length() - 1; i++ ) {
		    //std::cout << i << ", robot: " << state->getRobot().first << "," << state->getRobot().second << std::endl << ", waypoint: " << bob[i+1].first << "," << bob[i+1].second << std::flush;
		    goal = state->copySelf();
		    //std::cout << "Made copy -> goal" << std::endl << std::flush;
		    ret = goal->setRobot(bob[i+1]);
		    //std::cout << "Set the robot of goal" << std::endl << std::flush;
		    if (!ret)
		    {
			//std::cout << "Goal not free" << std::endl << std::flush;
			fitness = state->getScore();
			delete goal;
			delete state;
			//std::cout << "...quitting, fitness: " << fitness << std::endl << std::flush;
			return fitness*fitness;
		    }
		    //std::cout << "Start: " << std::endl << (*state) << std::endl << std::flush;
		    //std::cout << "Goal: " << bob[i+1].first << "," << bob[i+1].second << std::endl << (*goal) << std::endl << std::flush;
		    try
		    {
			nodes = waypointAStar(state, goal);
		    }
		    catch (int error)
		    {
			fitness = state->getScore();
			delete state;
			delete goal;
			return fitness*fitness;
		    }
		    delete state;
		    delete goal;
		    //std::cout << "Got nodes: " << nodes.size() << std::endl << std::flush;
		    state = nodes[nodes.size()-1];
		    // Clean up to avoid memory leaks
		    /*
		    for (int j = 0; j < nodes.size()-1; j++)
		    {
			delete nodes[i];
		    }
		    */
		}
		fitness = state->getScore();
		//std::cout << "Fitness: " << fitness*fitness << std::endl << std::flush;
		return fitness*fitness; // Give better fitness an extra advantage...
	}

}
	
MineState* transduceMineWithWalk(MineState* initialMine, Walk walk) {
    MineState* state = initialMine->copySelf();
    //std::cout << "Checking fitness" << std::endl;
    //std::cout << "path length: " << bob.length() << std::endl;
    MineState* goal;
    bool ret;
    std::vector<MineState*> nodes;
    // For each segment of the path...
    for( int i = 0; i < walk.length() - 1; i++ ) {
	//std::cout << i << ", robot: " << state->getRobot().first << "," << state->getRobot().second << std::endl << ", waypoint: " << bob[i+1].first << "," << bob[i+1].second << std::flush;
	goal = state->copySelf();
	//std::cout << "Made copy -> goal" << std::endl << std::flush;
	ret = goal->setRobot(walk[i+1]);
	//std::cout << "Set the robot of goal" << std::endl << std::flush;
	if (!ret)
	{
	    std::cout << "Goal not free: " << i << "/" << walk.length() << std::endl << std::flush;
	    delete goal;
	    //std::cout << "...quitting, fitness: " << fitness << std::endl << std::flush;
	    return state;
	}
	//std::cout << "Start: " << std::endl << (*state) << std::endl << std::flush;
	//std::cout << "Goal: " << bob[i+1].first << "," << bob[i+1].second << std::endl << (*goal) << std::endl << std::flush;
	try
	{
	    nodes = waypointAStar(state, goal);
	}
	catch (int error)
	{
	    std::cout << "A* failed" << std::endl;
	    delete goal;
	    return state;
	}
	delete state;
	delete goal;
	//std::cout << "Got nodes: " << nodes.size() << std::endl << std::flush;
	state = nodes[nodes.size()-1];
	// Clean up to avoid memory leaks
	/*
	  for (int j = 0; j < nodes.size()-1; j++)
	  {
	  delete nodes[i];
	  }
	*/
    }
    return state;
}

int main ()
{
    // Seed the random number generator.
    srand ( time(0) );

    // Read in the map
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    std::string input(buffer.str());
    MineState* initialMine = new NaiveMineState(input);

    //std::cout << "Read in map" << std::endl;

    walkBreeder::start = initialMine;
    walkBreeder::width = initialMine->getWidth();
    walkBreeder::height = initialMine->getHeight();
    GeneticAlgorithm< Walk > breeder(50, walkBreeder::fitness, walkBreeder::breed, walkBreeder::getRandomCreature );
    for( int i=0; i < 100; i++ ) {
	std::cout<< breeder.incrementGeneration() << std::endl;
	Walk bestCreature = (*breeder.getBestCreature());
	MineState* finalState = transduceMineWithWalk(initialMine, bestCreature);
	if (finalState != NULL) {
	    std::cout<< (*finalState) << std::endl;
	}
	else {
	    std::cout << "NULL!" << std::endl;
	}
    }

    /*
    Walk w = randomWalk( std::pair<int,int>( 0, 0 ), 10, 100, 30 );
    for( int i = 0; i < w.length(); i++ ) {
	std::cout<< w[i].first << " " << w[i].second << "\n";
    }
    */

    return 0;
}
