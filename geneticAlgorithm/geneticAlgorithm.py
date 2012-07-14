import numpy

def weighted_choice(weights):
	totals = numpy.cumsum(weights)
	norm = totals[-1]
	throw = numpy.random.rand()*norm
	return numpy.searchsorted(totals, throw)

class genetic_algorithm:
		
	def __init__( self, get_fitness, breed_creature, get_random_creature ):
		self.get_fitness = get_fitness
		self.breed_creature = breed_creature
		self.get_random_creature = get_random_creature

		self.population_size = 100
		self.current_population = []
		for creature_index in range( self.population_size ):
			self.current_population.append( self.get_random_creature() )
		self.historical_populations = []
	
	def increment_generation( self ):
		#calculate fitness
		fitness = []
		for creature in self.current_population :
			fitness.append( self.get_fitness( creature ) )

		#generate the new population
		new_population = []
		for new_creature_index in range( self.population_size ):
			new_population.append( self.breed_creature( self.current_population[ weighted_choice( fitness ) ], self.current_population[ weighted_choice( fitness ) ] ) )
		
		#tick the population forward
		self.historical_populations.append( self.current_population )
		self.current_population = new_population
