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
				waypoint.second += ( rand() % 3 - 1 );
			};
			child.append( waypoint );
		}
		return child;
	}

	float fitness( Walk bob ) {
		float fitness = 0;
		// For each segment of the path...
		for( int i = 0; i < bob.length() - 1; i++ ) {
			fitness += aStarSearch( bob[i], bob[ i + 1 ], planeSearch::equals, planeSearch::neighbors, planeSearch::heuristic ).size();
		}
		return fitness*fitness; // Give better fitness an extra advantage...
	}

}
	

int main ()
{
	// Seed the random number generator.
	srand ( time(0) );

	GeneticAlgorithm< Walk > breeder( 50, walkBreeder::fitness, walkBreeder::breed, walkBreeder::getRandomCreature );
	for( int i=0; i < 100; i++ ) {
		std::cout<< breeder.incrementGeneration() << "\n";
	}

	Walk w = randomWalk( std::pair<int,int>( 0, 0 ), 10, 100, 30 );
	for( int i = 0; i < w.length(); i++ ) {
		std::cout<< w[i].first << " " << w[i].second << "\n";
	}

	return 0;
}
