from dynamicSearch import dynamicSearch

print dynamicSearch( 30, lambda x: x == 0, lambda x: set( [ ( x-1, 1 ), ( x+1, 1 ) ] ), lambda x: x )


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
