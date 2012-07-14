#from geneticAlgorithm import GeneticAlgorithm
from search import dynamicSearch

#For simplicity, several desirable features will be left out of this test.
#We don't yet cache the best creature to return when time is called.
#We don't yet allow the cardinality of the path to vary. (Always 5 waypoints)
#We don't check to make sure all our waypoints are valid.
#If a waypoint is invalid, it will just cause the creature to get a score of whatever it had when the waypoint search failed.

def neighbors1( point ):
	(x,y) = point

	canidates = [
		(x-1, y),
		(x+1, y),
		(x, y-1),
		(x, y+1)
	]

	return set( [ (point, 1) for point in canidates] )

def manhattanDistance1( point ):
	(x,y) = point
	return abs(x) + abs(y)

def isGoal1( point ):
	(x,y) = point
	return ( x == 0 ) and ( y == 0 )

print dynamicSearch( (30,30), isGoal1, neighbors1, manhattanDistance1 )

class TestMap:
	def __init__( self, loc ):
		self.loc = loc
	def getLoc( self ):
		return self.loc

def neighbors( m ):
	(x,y) = m.getLoc()

	canidates = [
		(x-1, y),
		(x+1, y),
		(x, y-1),
		(x, y+1)
	]

	return set( [ (TestMap(point), 1) for point in canidates] )

def heuristic( m ):
	(x,y) = m.getLoc()
	return abs(x) + abs(y)

def isGoal( m ):
	(x,y) = m.getLoc()
	return ( x == 0 ) and ( y == 0 )

print [ mine.getLoc() for mine in dynamicSearch( TestMap( (30,30) ), isGoal, neighbors, heuristic ) ]

'''
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
'''
