#ifndef __SEARCH__
#define __SEARCH__

#define NO_PATH_FOUND -1

#include <vector>
#include <map>

// Take a pathMap and return the path from startNode to endNode.
// It is assumed that endNode is a valid key in pathMap and
// following
template <class Node>
std::vector<Node> reconstructPath( Node start, Node end, std::map<Node, Node> pathMap ) {
	std::vector<Node> path;
	Node current = end;
	while( current != start ) {
		path.insert( path.begin(), current );
		current = pathMap[ current ];
	}
	path.insert( path.begin(), start );
	return path;
}

#include "aStarSearch.h"

#endif
