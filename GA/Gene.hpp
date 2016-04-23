#pragma once
#include "Common.hpp"

template<typename GeneType>
class Gene
{
public:
    Gene()
            : aMinValue(0), aMaxValue(0), aValue(0)
    {
        /* FIXME, only needed at Crossover to create empty genes, that later will be assigned to Crossover result */
    }

    Gene(GeneType minValue, GeneType maxValue)
            : aMinValue(minValue), aMaxValue(maxValue), aValue(getRandomValue())
    {
    }

    /* Mutation */
    Gene<GeneType>& operator++();

    /* Clone */
    Gene<GeneType>& operator=(const Gene<GeneType>& other);

    /* Print */
    template<class GeneType2>
    friend ostream& operator<<(ostream& os, const Gene<GeneType2>& object);

    /* Optional */
    GeneType getValue() const
    {
        return aValue;
    }

protected:
private:
    GeneType aMinValue;
    GeneType aMaxValue;
    GeneType aValue;

    static const double MUTATION_PROBABILITY;
    static const double MUTATION_RESUS;

    /* Randomizer */
    GeneType getRandomValue();
    double getMutationResus();
    double getMutationProbability();
};

/* PRIVATE */

template<typename GeneType>
const double Gene<GeneType>::MUTATION_PROBABILITY = 0.01;
template<typename GeneType>
const double Gene<GeneType>::MUTATION_RESUS = 0.5;

/* Randomizer */

template<typename GeneType>
GeneType Gene<GeneType>::getRandomValue()
{
    static uniform_real_distribution<double> unif(aMinValue, aMaxValue);
    static default_random_engine re((unsigned)time(0));
    return unif(re);
}

template<typename GeneType>
double Gene<GeneType>::getMutationResus()
{
    static uniform_real_distribution<double> unif(0, 1);
    static default_random_engine re((unsigned)time(0));
    return unif(re);
}

template<typename GeneType>
double Gene<GeneType>::getMutationProbability()
{
    static uniform_real_distribution<double> unif(0, 1);
    static default_random_engine re((unsigned)time(0));
    return unif(re);
}

/* PUBLIC */

/* Mutation */

template<typename GeneType>
Gene<GeneType>& Gene<GeneType>::operator++()
{
    if (getMutationProbability() > MUTATION_PROBABILITY)
        return *this;

    aValue = (getMutationResus() > MUTATION_RESUS ? 1 : -1) * aValue * 0.1; // FIXME Magic numbers

    aValue = aValue < aMinValue ? aMinValue : aValue;
    aValue = aValue > aMaxValue ? aMaxValue : aValue;

    return *this;
}

/* Clone */

template<typename GeneType>
Gene<GeneType>& Gene<GeneType>::operator=(const Gene<GeneType>& other)
{
    aMinValue = other.aMinValue;
    aMaxValue = other.aMaxValue;
    aValue = other.aValue;
    return *this;
}

/* Print */

template<class GeneType>
ostream& operator<<(ostream& os, const Gene<GeneType>& object)
{
    return os << "Value: " << object.aValue << " [" << object.aMinValue << ", " << object.aMaxValue << "]";
}

