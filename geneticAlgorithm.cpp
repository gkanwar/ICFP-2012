//Picks an index of weights. A given index i is returned with probabilty p = weights[ i ] / sum( weights )
int weightedChoice( std::vector< float > weights ) {
    //std::cout << "Start weighted choice" << std::endl << std::flush;
    float sumOfWeights = 0;
    for(
        std::vector<float>::iterator j=weights.begin();
        j!=weights.end();
        ++j
    ) {
        sumOfWeights += *j;
    }

    if (sumOfWeights == 0)
    {
	return 0;
    }
    
    float location =  sumOfWeights*(float)rand()/(float)RAND_MAX;

    int index = 0;
    for(
        std::vector<float>::iterator j=weights.begin();
        location > 0;
        ++j
    ) {
        location -= *j;
        index++;
    }

    //std::cout << "End weighted choice: " << index-1 << std::endl << std::flush;
    return index - 1;
}

template <class Creature>
GeneticAlgorithm<Creature>::GeneticAlgorithm( int populationSize, float (*fitness)( Creature ), Creature (*breed)( Creature, Creature ),  Creature (*getRandomCreature)() ) {
    this->fitness = fitness;
    this->breed = breed;
    this->getRandomCreature = getRandomCreature;

    this->populationSize = populationSize;
    this->population = new Creature[ this->populationSize ]();
    for( int index = 0; index < this->populationSize; index ++ ) {
        this->population[ index ] = (*(this->getRandomCreature))();
    }
}

template <class Creature>
float GeneticAlgorithm<Creature>::incrementGeneration() {
    std::vector<float> fitnesses;
    float sumFitness = 0;
    for( int index = 0; index < this->populationSize; index ++ ) {
        float fitness = (*(this->fitness))( this->population[ index ] );
        fitnesses.push_back( fitness );
        sumFitness += fitness;
    }

    Creature* newPopulation = new Creature[ this->populationSize ]();
    for( int index = 0; index < this->populationSize; index ++ ) {
        Creature mom = this->population[ weightedChoice( fitnesses )];
        Creature pop = this->population[ weightedChoice( fitnesses )];
	//std::cout << "Breeding" << std::endl << std::flush;
        newPopulation[ index ] = (*(this->breed))( mom, pop );
	//std::cout << "Done Breeding" << std::endl << std::flush;
    }

    this->fossilRecord.push_back( this->population );
    this->population = newPopulation;

    return sumFitness / this->populationSize;
}

template <class Creature>
Creature* GeneticAlgorithm<Creature>::getBestCreature()
{
    Creature* best;
    float bestFitness = 0;
    for( int index = 0; index < this->populationSize; index ++ ) {
        float fitness = (*(this->fitness))( this->population[ index ] );
	if (fitness >= bestFitness) {
	    best = &(this->population[index]);
	    fitness = bestFitness;
	}
    }

    return best;
}
