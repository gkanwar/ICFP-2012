#ifndef __SEARCH__
#define __SEARCH__

#include <vector>
#include <map>
#include <algorithm>

// Error codes

// Returned if there is no valid path
// on the graph given.
#define NO_PATH_FOUND -1

// Struct that describes the edge from one node to another.
template <class Node>
struct Edge {

	// The node at the end of the edge.
	Node target;

	// The cost of transversing the node.
	float cost;

	// Simple constructor.
	Edge( Node, float );
};

// Take a pathMap and return the path from start to end.
// It is assumed that end is a valid key in pathMap and
// following pathMap backwards will get to start
template <class Node>
std::vector<Node> reconstructPath( Node start, Node end, std::map<Node, Node> pathMap );

#include "dijkstraSearch.h"
#include "aStarSearch.h"

#include "search.cpp"
#endif
