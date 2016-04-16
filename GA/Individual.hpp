
#pragma once
#include "Common.hpp"



#include "Chromosome.hpp"
#include "UtilityEvaluator.hpp"



const int DEFAULT_GENE_COUNT = 6;



template <typename GeneType>
class Individual : public UtilityEvaluator
{
public:
	Individual() : Individual(DEFAULT_GENE_COUNT, -1, 1) {}
	Individual(size_t geneCount, GeneType minValue, GeneType maxValue) : aGenome(geneCount) { aMultipliers = aGenome.getValues(); }
	Individual(const Chromosome<GeneType>& genome) : aGenome(genome) { aMultipliers = aGenome.getValues(); }

	/* Crossover */
	pair<Individual, Individual> operator+(const Individual& other);

	/* Clone*/
	Individual& operator=(const Individual& other);

	/* Print */
	template <class _GeneType>
	friend ostream& operator<<(ostream& os, const Individual<_GeneType>& object) { os << object.aGenome << endl; return os; }

protected:

private:
	Chromosome<double> aGenome;

};

/* PUBLIC */

/* CROSSOVER */
template <typename GeneType>
pair<Individual<GeneType>, Individual<GeneType>> Individual<GeneType>::operator+(const Individual& other)
{
	pair<Chromosome<GeneType>, Chromosome<GeneType>> childs = aGenome + other.aGenome;
	return pair<Individual, Individual>(childs.first, childs.second);
}

/* CLONE */
template <typename GeneType>
Individual<GeneType>& Individual<GeneType>::operator=(const Individual& other)
{
	aGenome = other.aGenome;
	return *this;
}
