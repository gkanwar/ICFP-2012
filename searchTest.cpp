#include <iostream>

#include "search.h"

#include <cmath>

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

namespace intSearch {
	bool isGoal( int node ) {
		return node == 0;
	}

	std::vector< Edge< int > > neighbors( int node ) {
		std::vector< Edge< int > > frontier;

		frontier.push_back( Edge< int >( node + 1, 1 ) );
		frontier.push_back( Edge< int >( node - 1, 1 ) );

		return 	frontier;
	}

	float heuristic( int node ) {
		return std::abs( node );
	}
}

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

int main ()
{
	//Use A* to do a trivial search.
	try {
		print< int >( aStarSearch( 30, intSearch::isGoal, intSearch::neighbors, intSearch::heuristic ) );
	} catch( int error ) {
		std::cout<< "A* int search example failed.\n";
	}

	//Use Dijkstra's Algorithm to do a trivial search.
	try {
		print< int >( dijkstraSearch( 30, intSearch::isGoal, intSearch::neighbors ) );
	} catch( int error ) {
		std::cout<< "Dijkstra's int search example failed.\n";
	}

	return 0;
}
