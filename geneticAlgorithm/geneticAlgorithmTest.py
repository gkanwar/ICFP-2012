from genetic_algorithm import *
import numpy
import math
from pylab import *

# an example where creatures are numbers. They seek to become as large as possible.
def get_random_creature():
	return math.fabs( numpy.random.normal() )

def get_fitness( creature ):
	return creature

def breed_creature( creatureA, creatureB ):
	return math.fabs( creatureA + creatureB + numpy.random.normal() )/2

for run in range(20):
	g= genetic_algorithm( get_fitness,  breed_creature, get_random_creature )	
	average_fitness = []
	for index in range(10000):
		average_fitness.append( numpy.mean( g.current_population ) )
		g.increment_generation()
	plot( average_fitness )
show()
