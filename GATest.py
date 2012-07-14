from map import NaiveMap
from geneticAlgorithm import GeneticAlgorithm
from search import dynamicSearch

#For simplicity, several desirable features will be left out of this test.
#We don't yet cache the best creature to return when time is called.
#We don't yet allow the cardinality of the path to vary. (Always 5 waypoints)
#We don't check to make sure all our waypoints are valid.
#If a waypoint is invalid, it will just cause the creature to get a score of whatever it had when the waypoint search fails.

print "Testing Map!"
m = NaiveMap("""
######
#. *R#
#  \.#
#\ * #
L  .\#
######
""".strip('\n'))

print m

print "Testing Search!"

print dynamicSearch( 30, lambda x: x == 0, lambda x: set( [ ( x-1, 1 ), ( x+1, 1 ) ] ), lambda x: x )

class TESTMAP:
	this.location
# Returns a pruned list of neighbors of a map;
# e.g. maps that can be reached after the robot
# takes a single move. Only "useful" maps are
# returned (e.g. the robot cannot abort).
def neighbors( map ):
	validCommands = [ 'U', 'D', 'L', 'R', 'W' ]
	neighbors = [ update( map, command ) for command in validCommands ]
	#TODO prune neighbors.
	return [ ( neighbor, 1 ) for neighbor in neighbors ]

# Returns a function object that will return true
# iff the robot is at the goal position in the
# map passed in.
def makeIsGoal( goal ):
	return lambda map: goal == map.getRobotLocation()

# Returns a function object which will return the
# manhattan distance between the robot and the
# goal in the map. Since the robot can make one
# move each step at most, this heuristic is permissible.
def makeHeuristic( goal ):
	return lambda map: manhattanDistance( goal, map.getRobotLocation() )

#DUMMY FXN
def update( map, command ):
	return (map, command)

# Calculates the manhattan distance from pointA
# to pointB, i.e. the sum of delta X and delta Y. 
def manhattanDistance( pointA, pointB ):
	return abs( pointA[0] - pointB[0] ) + abs( pointA[1] - pointB[1] )

m.transduce('LDRDDUULLLDDL')

print m

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

 
