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

int main ()
{
	//Use A* to do a trivial search.
	try {
		print< int >( aStarSearch( 30, intSearch::isGoal, intSearch::neighbors, intSearch::heuristic ) );
	} catch( int error ) {
		std::cout<< "A* int search example failed.\n";
	}
	return 0;
}
