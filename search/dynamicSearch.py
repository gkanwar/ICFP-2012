def dynamicSearch( start, isGoal, neighbors, heuristic ):
	# Set of nodes we're still looking at.
	openNodes = set([ start ])
	# Set of nodes we've explored.
	visitedNodes = set()
	# Map of explored nodes.
	pathMap	= { start:None }

	# Costs to get to each node.
	knownCost = { start:0 }
	# Optimistic estimant of the cost to goal through each explored node.
	estCost	= { start:heuristic( start ) }

	while len( openNodes ) > 0:

		# Pick the node in open with the lowest estimated cost.
		minEstCost = None
		current = None
		for node in openNodes:
			if ( minEstCost is None ) or ( estCost[ node ] < minEstCost ):
				minEstCost = estCost[ node ]
				current = node

		# Check if we've reached the goal...
		if isGoal( current ):
			return reconstructPath( current, pathMap )

		# Move current to the visited set...
		openNodes.remove( current )
		visitedNodes.add( current )

		for ( neighbor, cost ) in neighbors( current ):
			if neighbor in visitedNodes:
				continue

			if ( not neighbor in openNodes ) or ( cost < knownCost[ neighbor ] - knownCost[ current ] ):
				openNodes.add( neighbor )
				pathMap[ neighbor ] = current
				knownCost[ neighbor ] = cost + knownCost[ current ];
				estCost[ neighbor ] = knownCost[ neighbor ] + heuristic( neighbor )

	return None;
			
			

def reconstructPath( endNode, pathMap ):
	path = [endNode]
	while not pathMap[ path[0] ] is None:
		path.insert(0, pathMap[ path[0] ] )
	return path
	
