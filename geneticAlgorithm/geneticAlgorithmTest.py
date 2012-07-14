from geneticAlgorithm import *
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

print "While you wait, a neat aside.\n\n(AKA this could take a minute. It's mostly the plotting.)\n\nWhen the plot comes up, you'll see that the curves seem to approximate C*sqrt( generation ) this is in fact the case! It's a neat little quick diffeq problem if you look at the code. Keep yourself amused. Seriously, this plotter can be slow as fuck, since it plots every generation for all 10 runs. I really should fix that. Still. Get a pencil and do the math. See if you can finish before it does. I dare you.\n\n~Will"

for run in range(10):
	g= GeneticAlgorithm( getFitness,  breedCreature, getRandomCreature )	
	averageFitness = []
	for index in range(100):
		averageFitness.append( numpy.mean( g.currentPopulation ) )
		g.incrementGeneration()
	plot( averageFitness )
show()
