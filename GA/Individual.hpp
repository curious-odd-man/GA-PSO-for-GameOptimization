#pragma once
#include "Common.hpp"

#include "Chromosome.hpp"
#include "UtilityEvaluator.hpp"

template<typename GeneType>
class Individual : public UtilityEvaluator
{
public:
    Individual(size_t geneCount, GeneType minValue, GeneType maxValue)
            : aGenome(geneCount, minValue, maxValue)
    {
        aMultipliers = aGenome.getValues();
    }

    Individual(const Chromosome<GeneType>& genome)
            : aGenome(genome)
    {
        aMultipliers = aGenome.getValues();
    }

    /* Crossover */
    pair<Individual, Individual> operator+(const Individual& other);

    /* Clone*/
    Individual& operator=(const Individual& other);

    /* Print */
    template<class _GeneType>
    friend ostream& operator<<(ostream& os, const Individual<_GeneType>& object)
    {
        os << object.aGenome << endl;
        return os;
    }

protected:

private:
    Chromosome<GeneType> aGenome;

};

/* PUBLIC */

/* CROSSOVER */
template<typename GeneType>
pair<Individual<GeneType>, Individual<GeneType>> Individual<GeneType>::operator+(const Individual& other)
{
    pair<Chromosome<GeneType>, Chromosome<GeneType>> childs = aGenome + other.aGenome;
    return pair<Individual, Individual>(childs.first, childs.second);
}

/* CLONE */
template<typename GeneType>
Individual<GeneType>& Individual<GeneType>::operator=(const Individual& other)
{
    aGenome = other.aGenome;
    return *this;
}
