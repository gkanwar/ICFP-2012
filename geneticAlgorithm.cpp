//Picks an index of weights. A given index i is returned with probabilty p = weights[ i ] / sum( weights )
int weightedChoice( std::vector< float > weights ) {

    float sumOfWeights = 0;
    for(
        std::vector<float>::iterator j=weights.begin();
        j!=weights.end();
        ++j
    ) {
        sumOfWeights += *j;
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

    return index - 1;
}

template <class Creature>
GeneticAlgorithm<Creature>::GeneticAlgorithm( float (*fitness)( Creature ), Creature (*breed)( Creature, Creature ),  Creature (*getRandomCreature)() ) {
    this->fitness = fitness;
    this->breed = breed;
    this->getRandomCreature = getRandomCreature;

    this->populationSize = 1000;
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
        newPopulation[ index ] = (*(this->breed))( mom, pop );
    }

    this->fossilRecord.push_back( this->population );
    this->population = newPopulation;

    return sumFitness / this->populationSize;
}
