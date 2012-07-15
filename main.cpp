#include <iostream>

#include "geneticAlgorithm.h"
#include "search.h"
#include "mine.h"

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
	#define GOAL std::pair<int, int>( 0, 0 )
	bool isGoal( std::pair<int, int> node ) {
		return ( node.first == GOAL.first && node.second == GOAL.second );
	}

	std::vector< Edge< std::pair<int, int> > > neighbors( std::pair<int, int> node ) {
		std::vector< Edge< std::pair<int, int> > > frontier;

		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first + 1, node.second ), 1 ) );
		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first - 1, node.second ), 1 ) );
		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first, node.second + 1 ), 1 ) );
		frontier.push_back( Edge< std::pair<int, int> >( std::pair<int, int>( node.first, node.second - 1 ), 1 ) );
		
		return 	frontier;
	};

	float heuristic( std::pair<int, int> node ) {
		return manhattanDistance( node, GOAL );
	};
}

namespace walkBreeder {
	//TODO: Make these dynamic
	#define START std::pair< int, int >( 0, 0 )
	#define WIDTH 30
	#define HEIGHT 30
	#define WALKLENGTH 10
	Walk getRandomCreature() {
		return randomWalk( START, HEIGHT, WIDTH, WALKLENGTH );
	}

	Walk breed( Walk mom, Walk pop ) {
		Walk child( START );
		//TODO: randomness!
		for( int index = 1; index < WALKLENGTH; index++ ) {
			if( rand() % 2 ) {
				child.append( mom[ index ] );
			}
			else {
				child.append( pop[ index ] );
			}
		}
		return child;
	}

	float fitness( Walk bob ) {
		float fitness = 0;
		for( int i = 0; i < bob.length(); i++ ) {
			fitness += bob[i].first + bob[i].second;
		}
		return fitness;
	}
}
	

int main ()
{
	// Seed the random number generator.
	srand ( time(0) );

	GeneticAlgorithm< Walk > breeder( 1000, walkBreeder::fitness, walkBreeder::breed, walkBreeder::getRandomCreature );
	for( int i=0; i < 500; i++ ) {
		std::cout<< breeder.incrementGeneration() << "\n";
	}

	//Use A* to do a trivial search.
	try {
		print< std::pair< int, int > >( aStarSearch( std::pair<int, int>( 30, 30 ), planeSearch::isGoal, planeSearch::neighbors, planeSearch::heuristic ) );
	} catch( int error ) {
		std::cout<< "A* int search example failed.\n";
	}

	Walk w = randomWalk( std::pair<int,int>( 0, 0 ), 10, 100, 30 );
	for( int i = 0; i < w.length(); i++ ) {
		std::cout<< w[i].first << " " << w[i].second << "\n";
	}

	return 0;
}
