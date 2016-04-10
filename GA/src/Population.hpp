
#pragma once
#include "Common.hpp"



#include "Individual.hpp"
#include "Game.hpp"



const size_t POPULATION_DENSITY = 100;
const size_t GENERATIONS = 100;
const size_t FIELD_WIDTH = 7;




class Population
{
public:
	Population() : aPopulationDensity(POPULATION_DENSITY), aGenerations(GENERATIONS), aPopulation(aPopulationDensity) {}

    void live()
    {
        ofstream out;
        out.open("dump.log");
        out << "ListPlot[{";

        vector<OptimizationGame> games;

        int i = 0;
        for (auto& individual : aPopulation)
        {
            games.emplace_back(individual);
            ++i;
        }

        for (size_t i = 0; i < aGenerations; ++i)
        {
            for (size_t j = 0; j < aPopulationDensity; ++j)
                games[j].play(&aPopulation[j]);


            Individual& individual = *max_element(aPopulation.begin(), aPopulation.end(), [](const Individual& a, const Individual& b) -> bool
            {
                return a.getUtility() < b.getUtility();
            });
            out << "{ " << i << ", " << individual.getUtility() << " }";

            if (i == aGenerations - 1)
                break;
            out << ", ";
            evolve();
        }

        out << "}]" << endl;
        out.close();
    }

	void evolve()
	{
        setEvolutionLaw(accumulate(aPopulation.begin(), aPopulation.end(), 0ULL, [](const size_t& a, const Individual& b) { return a + max(b.getUtility(), (size_t)1); }));

		vector<Individual> newGeneration;

        for (size_t i = 0; i < aPopulationDensity / 2; ++i)
        {
            Individual& lucky1 = getLucky();
            Individual& lucky2 = getLucky();
            const Individual& unprotected_sex_result = lucky1 + lucky2;
            newGeneration.push_back(unprotected_sex_result);
            const Individual& unprotected_sex_result2 = lucky2 + lucky1;
            newGeneration.push_back(unprotected_sex_result2);
        }

		aPopulation = newGeneration;
	}

	/* Print */
	friend ostream& operator<<(ostream& os, const Population& object)
	{
		for (auto individual : object.aPopulation)
			os << individual << endl;

		return os;
	}

protected:

private:
	size_t aPopulationDensity;
	size_t aGenerations; // https://www.youtube.com/watch?v=v0NSeysrDYw
	vector<Individual> aPopulation;

	/**/
	uniform_int_distribution<size_t> sectors;
	default_random_engine roulette;

	void setEvolutionLaw(size_t wholeUtility)
	{
		sectors = uniform_int_distribution<size_t>(0, wholeUtility);
		roulette = default_random_engine((unsigned int)(time(0) + wholeUtility));
	}

	Individual& getLucky()
	{
		size_t start = 0;
		size_t lucky = sectors(roulette);

		return *find_if(aPopulation.begin(), aPopulation.end(), [&start, lucky](const Individual& a) mutable
		{
			start += max(a.getUtility(), (size_t)1);
			if (lucky <= start)
				return true;

			return false;
		});
	}
};
