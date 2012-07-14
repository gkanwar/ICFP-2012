import numpy

# TODO: Rewrite this so it doesn't depend on numpy and is faster.
# Right now it's a huge time sink.
def weightedChoice(weights):
	totals = numpy.cumsum(weights)
	norm = totals[-1]
	throw = numpy.random.rand()*norm
	return numpy.searchsorted(totals, throw)

class GeneticAlgorithm:
		
	def __init__( self, getFitness, breedCreature, getRandomCreature ):
		self.getFitness = getFitness
		self.breedCreature = breedCreature
		self.getRandomCreature = getRandomCreature

		self.populationSize = 100
		self.currentPopulation = []
		for creatureIndex in range( self.populationSize ):
			self.currentPopulation.append( self.getRandomCreature() )
		self.historicalPopulations = []
	
	def incrementGeneration( self ):
		#calculate fitness
		fitness = []
		for creature in self.currentPopulation :
			fitness.append( self.getFitness( creature ) )

		#generate the new population
		newPopulation = []
		for newCreatureIndex in range( self.populationSize ):
			newPopulation.append( self.breedCreature( self.currentPopulation[ weightedChoice( fitness ) ], self.currentPopulation[ weightedChoice( fitness ) ] ) )
		
		#tick the population forward
		self.historicalPopulations.append( self.currentPopulation )
		self.currentPopulation = newPopulation
