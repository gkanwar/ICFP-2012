from map import NaiveMap
from geneticAlgorithm import GeneticAlgorithm
from search import dynamicSearch

#For simplicity, several desirable features will be left out of this test.
#We don't yet cache the best creature to return when time is called.
#We don't yet allow the cardinality of the path to vary. (Always 5 waypoints)
#We don't check to make sure all our waypoints are valid.
#If a waypoint is invalid, it will just cause the creature to get a score of whatever it had when the waypoint search fails.

print "Testing Map!"

print "Testing Search!"

print dynamicSearch( 30, lambda x: x == 0, lambda x: set( [ ( x-1, 1 ), ( x+1, 1 ) ] ), lambda x: x )


obstacles = set()
for i in range( -10, 100 ):
	obstacles.add( ( i, 5 ) )
	obstacles.add( ( -10, i ) )

def neighbors( point ):
	(x,y) = point

	canidates = [
		(x-1, y),
		(x+1, y),
		(x, y-1),
		(x, y+1)
	]

	for canidate in canidates:
		if canidate in obstacles:
			canidates.remove( canidate )

	return set( [ (point, 1) for point in canidates] )

def manhattanDistance( point ):
	(x,y) = point
	return abs(x) + abs(y)

def isGoal( point ):
	(x,y) = point
	return ( x == 0 ) and ( y == 0 )

print dynamicSearch( (20,20), isGoal, neighbors, manhattanDistance )

print "Testing GA!"
import numpy
import math
from pylab import *

# an example where creatures are numbers. They seek to become as large as possible.
def getRandomCreature():
	return math.fabs( numpy.random.normal() )

def getFitness( creature ):
	return creature

def breedCreature( creatureA, creatureB ):
	return math.fabs( creatureA + creatureB + numpy.random.normal() )/2

for run in range(5):
	g= GeneticAlgorithm( getFitness,  breedCreature, getRandomCreature )	
	averageFitness = []
	for index in range(100):
		averageFitness.append( numpy.mean( g.currentPopulation ) )
		g.incrementGeneration()
	plot( averageFitness )
show()

 
