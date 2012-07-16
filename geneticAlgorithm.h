#ifndef __GENETIC_ALGORITHM__
#define __GENETIC_ALGORITHM__

#include <vector>
#include <stdlib.h>
#include <time.h>

template <class Creature>
class GeneticAlgorithm {
private:
    //Dynamically assigned member functions...
    float (*fitness)( Creature );
    Creature (*breed)( Creature, Creature );
    Creature (*getRandomCreature)();

    //Member Data
    int populationSize;
    Creature* population;
    std::vector<Creature*> fossilRecord;
public:
    GeneticAlgorithm( int, float (*fitness)( Creature ), Creature (*breed)( Creature, Creature ),  Creature (*getRandomCreature)() );
    float incrementGeneration();
    Creature* getBestCreature();
};

#include "geneticAlgorithm.cpp"

#endif
