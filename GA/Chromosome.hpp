#pragma once
#include "Common.hpp"
#include "Gene.hpp"

template<typename GeneType>
class Chromosome
{
public:
    Chromosome(size_t geneCount, GeneType minValue, GeneType maxValue)
            : aGeneCount(geneCount)
    {
        for (size_t i = 0; i < aGeneCount; ++i)
            aGenes.emplace_back(Gene<GeneType>(minValue, maxValue));
    }

    Chromosome(size_t geneCount, vector<Gene<GeneType>>& genes)
            : aGeneCount(geneCount), aGenes(genes)
    {
    }

    /* Move */
    Chromosome(Chromosome&& other)
            : aGeneCount(other.aGeneCount), aGenes(move(other.aGenes))
    {
    }

    /* Clone */
    Chromosome(const Chromosome& other)
            : aGeneCount(other.aGeneCount)
    {
        aGenes.clear();
        aGenes.assign(other.aGenes.begin(), other.aGenes.end());
    }

    /* Mutation */
    Chromosome<GeneType>& operator++();

    /* Crossover */
    pair<Chromosome<GeneType>, Chromosome<GeneType>> operator+(const Chromosome<GeneType>& other);

    /* Clone */
    Chromosome<GeneType>& operator=(const Chromosome<GeneType>& other);

    /* Print */
    template<class GeneType2>
    friend ostream& operator<<(ostream& os, const Chromosome<GeneType2>& object);

    /* Optional */
    vector<GeneType> getValues();

protected:
private:
    size_t aGeneCount;
    vector<Gene<GeneType>> aGenes;

    /* Randomizer */
    size_t getCrossoverRandom();
};

/* PRIVATE */

/* Randomizer */

template<typename GeneType>
size_t Chromosome<GeneType>::getCrossoverRandom()
{
    static uniform_int_distribution<int> unif(1, (int)aGeneCount - 1); // TODO: maybe they should be static too ?
    static default_random_engine re((unsigned)time(0));
    return (size_t)unif(re);
}

/* PUBLIC */

/* Crossover */

template<typename GeneType>
pair<Chromosome<GeneType>, Chromosome<GeneType>> Chromosome<GeneType>::operator+(const Chromosome<GeneType>& other)
{
    vector<Gene<GeneType>> genes1(aGeneCount);
    vector<Gene<GeneType>> genes2(aGeneCount);

    size_t algorithm = getCrossoverRandom();

    for (size_t i = 0; i < algorithm; i++)
    {
        // AB|CDEF + GH|IJKL = AB|IJKL & GH|CDEF
        //genes1[i] = aGenes[i];
        //genes2[i] = other.aGenes[i];

        // AB|CDEF + GH|IJKL = AB|IJKL & CDEF|GH
        genes1[i] = aGenes[i];
        genes2[aGeneCount - algorithm + i] = other.aGenes[i];
    }

    for (size_t i = algorithm; i < aGeneCount; i++)
    {
        // AB|CDEF + GH|IJKL = AB|IJKL & GH|CDEF
        //genes1[i] = other.aGenes[i];
        //genes2[i] = aGenes[i];

        // AB|CDEF + GH|IJKL = AB|IJKL & CDEF|GH
        genes1[i] = other.aGenes[i];
        genes2[i - algorithm] = aGenes[i];
    }


    return pair<Chromosome<GeneType>, Chromosome<GeneType>>(++Chromosome(aGeneCount, genes1),
                                                            ++Chromosome(aGeneCount, genes2));
}

template<typename GeneType>
Chromosome<GeneType>& Chromosome<GeneType>::operator=(const Chromosome<GeneType>& other)
{
    aGeneCount = other.aGeneCount;

    aGenes.clear();
    aGenes.assign(other.aGenes.begin(), other.aGenes.end());

    return *this;
}

/* Mutation */

template<typename GeneType>
Chromosome<GeneType>& Chromosome<GeneType>::operator++()
{
    for (size_t i = 0; i < aGeneCount; ++i)
        ++(aGenes[i]);

    return *this;
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
