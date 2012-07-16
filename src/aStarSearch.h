#ifndef __A_STAR_SEARCH__
#define __A_STAR_SEARCH__

template <class Node>
bool findInVector(std::vector< Node > nodes, bool (*equals)( Node, Node ), Node obj)
{
    for (int i = 0; i < nodes.size(); i++)
    {
	if (equals(nodes[i], obj))
	{
	    return true;
	}
    }

    return false;
}

template <class Node>
std::vector<Node> aStarSearch( Node start, Node goal, bool (*equals)( Node, Node ), std::vector< Edge< Node > > (*neighbors)( Node ),  float (*heuristic)( Node, Node ) ) {

	// Set of nodes we're still looking at.
	std::vector< Node > openNodes;
	openNodes.push_back( start );
	// Set of nodes we've explored.
	std::vector< Node > visitedNodes;
	
	//Map of explored nodes.
	std::map< Node, Node > pathMap;

	// Costs to get to each node.
	std::map< Node, float > knownCost;
	knownCost[ start ] = 0;
	// Optimistic estimant of the cost to goal through each explored node.
	std::map< Node, float > estCost;
	estCost[ start ] = (*heuristic)( start, goal );

	while( openNodes.size() > 0 ) {

		// Pick the node in openNodes with the lowest estimated cost.
		float minEstCost = -1;
		Node current;
		typename std::vector< Node >::iterator currentItr;

		for( typename std::vector< Node >::iterator node = openNodes.begin(); node != openNodes.end(); ++node) {
    			if( ( minEstCost == -1 ) || ( estCost[ *node ] < minEstCost ) ) {
				minEstCost = estCost[ *node ];
				current = *node;
				currentItr = node;
			}
		}

		openNodes.erase( currentItr );
		visitedNodes.push_back( current );

		// Check if we've reached the goal...
		if( equals( current, goal ) ) {
			return reconstructPath<Node>( start, current, pathMap );
		};

		std::vector< Edge< Node > > frontier = (*neighbors)( current );

		for( typename std::vector< Edge< Node > >::iterator it = frontier.begin(); it != frontier.end(); ++it) {
			Node neighbor = it->target;
			float cost = it->cost;
			// Make sure we haven't visited this node before.
			if( findInVector(visitedNodes, equals, neighbor) ) {
				continue;
			}

			// Add this neighbor to open nodes if appropriate.
			if(
			    ( !findInVector( openNodes, equals, neighbor ) )
				|| ( cost < knownCost[ neighbor ] - knownCost[ current ] )
			) {
				openNodes.push_back( neighbor );
				pathMap[ neighbor ] = current;
				knownCost[ neighbor ] = cost + knownCost[ current ];
				estCost[ neighbor ] = knownCost[ neighbor ] + (*heuristic)( neighbor, goal  );
			}
		}
	}

	// If we ever get here, we've exauhsted the space without
	// finding our goal. Throw an error message.	
	throw NO_PATH_FOUND;
};

#endif
