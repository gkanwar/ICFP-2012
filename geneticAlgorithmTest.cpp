#include <iostream>

#include "geneticAlgorithm.h"

float getRandomCreature() {
    return (float)rand()/(float)RAND_MAX;
}

float breed( float mom, float pop ) {
    return ( mom + pop ) / 2 + (float)rand()/(float)RAND_MAX - 0.5;
}

float fitness( float bob ) {
    return bob;
}

int main ()
{
    srand ( time(0) );
    GeneticAlgorithm< float > floatBreeder( fitness, breed, getRandomCreature );
    for( int i=0; i < 100; i++ ) {
        std::cout<< floatBreeder.incrementGeneration() << "\n";
    }
    return 0;
}
