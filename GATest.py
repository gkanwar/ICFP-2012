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

def neighbors( map ):
	validCommands = [ 'U', 'D', 'L', 'R' ]
	for command in validCommands:
		state, ( status, score ) = map.step( command, state=None, update=False, pprint=False );
		print NaiveMap.pprint( state ), status, score
	
print neighbors( m )

def makeIsGoal( goal ):
	return lambda map: goal == map.state[2]

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

 
