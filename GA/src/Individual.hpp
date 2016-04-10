
#pragma once
#include "Common.hpp"



#include "Chromosome.hpp"
#include "UtilityEvaluator.hpp"



class Individual : public UtilityEvaluator
{
public:
	Individual() : aGenome(6, -1, 1) { aMultipliers = aGenome.getValues(); }

	Individual(const Chromosome<double>& genome) : aGenome(genome) { aMultipliers = aGenome.getValues(); }

	Individual operator+(const Individual& other);

	Individual& operator=(const Individual& other);

	/* Print */
	friend ostream& operator<<(ostream& os, const Individual& object) { os << object.aGenome << endl; return os; }

protected:

private:
	Chromosome<double> aGenome;

	void setMultipliers();
};

void Individual::setMultipliers()
{
	aMultipliers = aGenome.getValues();
}

Individual Individual::operator+(const Individual& other)
{
	return Individual(++(aGenome + other.aGenome));
}

Individual& Individual::operator=(const Individual& other)
{
	aGenome = other.aGenome;
	return *this;
}

