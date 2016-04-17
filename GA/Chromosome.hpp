#pragma once
#include "Common.hpp"

#include "Gene.hpp"

const double MUTATION_PROBABILITY = 0.05;

template<typename GeneType>
class Chromosome
{
public:
    Chromosome(size_t geneCount)
            : aGeneCount(geneCount), aGenes(new Gene<GeneType> [aGeneCount]())
    {
    }

    Chromosome(size_t geneCount, Gene<GeneType>* genes)
            : aGeneCount(geneCount), aGenes(genes)
    {
    }

    Chromosome(Chromosome&& other)
            : aGeneCount(other.aGeneCount), aGenes(other.aGenes)
    {
        other.aGenes = nullptr;
    }

    Chromosome(const Chromosome& other)
            : aGeneCount(other.aGeneCount)
    {
        aGenes = new Gene<GeneType> [aGeneCount]();

        for (size_t i = 0; i < aGeneCount; ++i)
            aGenes[i] = other.aGenes[i];
    }

    ~Chromosome()
    {
        delete[] aGenes;
    }

    /* Mutation */
    Chromosome<GeneType>& operator++();
    Chromosome<GeneType> operator++(int);

    /* Crossover */
    pair<Chromosome<GeneType>, Chromosome<GeneType>> operator+(const Chromosome<GeneType>& other);

    /* Clone */
    Chromosome<GeneType>& operator=(const Chromosome<GeneType>& other);

    /* Move */
    Chromosome<GeneType>& operator=(Chromosome<GeneType> && other);

    /* Print */
    template<class GeneType2>
    friend ostream& operator<<(ostream& os, const Chromosome<GeneType2>& object);

    /**/
    vector<GeneType> getValues();

protected:
private:
    size_t aGeneCount;
    Gene<GeneType>* aGenes;

    double getRandom(double);
    int getMutationRandom();
    int getCrossoverRandom();
};

/* Private methods */

/* Randomizer */

template<typename GeneType>
double Chromosome<GeneType>::getRandom(double)
{
    static uniform_real_distribution<double> unif(0, 1); // TODO: maybe they should be static too ?
    static default_random_engine re((unsigned) time(0));
    return unif(re);
}

template<typename GeneType>
int Chromosome<GeneType>::getMutationRandom()
{
    static uniform_int_distribution<int> unif(0, (int) aGeneCount); // TODO: maybe they should be static too ?
    static default_random_engine re((unsigned) time(0));
    return unif(re);
}

template<typename GeneType>
int Chromosome<GeneType>::getCrossoverRandom()
{
    static uniform_int_distribution<int> unif(1, (int) aGeneCount - 1); // TODO: maybe they should be static too ?
    static default_random_engine re((unsigned) time(0));
    return unif(re);
}

/* Public methods */

template<typename GeneType>
pair<Chromosome<GeneType>, Chromosome<GeneType>> Chromosome<GeneType>::operator+(const Chromosome<GeneType>& other)
{
    Gene<GeneType>* genes1 = new Gene<GeneType> [aGeneCount]();
    Gene<GeneType>* genes2 = new Gene<GeneType> [aGeneCount]();

    size_t algorithm = getCrossoverRandom();

    for (size_t i = 0; i < algorithm; i++)
    {
        genes1[i] = aGenes[i];
        genes2[i] = other.aGenes[i];
    }

    for (size_t i = algorithm; i < aGeneCount; i++)
    {
        genes1[i] = other.aGenes[i];
        genes2[i] = aGenes[i];
    }

    return pair<Chromosome<GeneType>, Chromosome<GeneType>>(++Chromosome(aGeneCount, genes1),
                                                            ++Chromosome(aGeneCount, genes2));
}

template<typename GeneType>
Chromosome<GeneType>& Chromosome<GeneType>::operator=(const Chromosome<GeneType>& other)
{
    aGeneCount = other.aGeneCount;

    delete[] aGenes;
    aGenes = new Gene<GeneType> [aGeneCount]();

    for (size_t i = 0; i < aGeneCount; ++i)
        aGenes[i] = other.aGenes[i];

    return *this;
}

template<typename GeneType>
Chromosome<GeneType>& Chromosome<GeneType>::operator=(Chromosome<GeneType> && other)
{
    aGeneCount = other.aGeneCount;
    aGenes = other.aGenes;

    other.aGenes = nullptr;

    return *this;
}

/* Mutation */

template<typename GeneType>
Chromosome<GeneType>& Chromosome<GeneType>::operator++()
{
    if (getRandom((double) 0) < MUTATION_PROBABILITY)
    {
        int a = getMutationRandom();
        aGenes[a]++;
    }
    return *this;
}

template<typename GeneType>
Chromosome<GeneType> Chromosome<GeneType>::operator++(int)
{
    Chromosome<GeneType> tmp(*this);
    operator++();
    return tmp;
}

/* Print */
template<class GeneType>
ostream& operator<<(ostream& os, const Chromosome<GeneType>& object)
{
    for (size_t i = 0; i < object.aGeneCount; i++)
        os << object.aGenes[i] << endl;
    return os;
}

/**/
template<typename GeneType>
vector<GeneType> Chromosome<GeneType>::getValues()
{
    vector<GeneType> tmp;

    for (size_t i = 0; i < aGeneCount; ++i)
        tmp.emplace_back(aGenes[i].getValue());

    return tmp;
}
