#pragma once
#include "Common.hpp"

#include "Individual.hpp"
#include "OptimizationGame.hpp"

const size_t DEFAULT_POPULATION_DENSITY = 50;
const size_t DEFAULT_GENERATIONS = 50;

template<typename GeneType>
    class Population
    {
    public:
        Population ()
                : Population (DEFAULT_POPULATION_DENSITY, DEFAULT_GENERATIONS)
        {
        }
        Population (size_t populationsDensity, size_t generations)
                : aPopulationDensity (populationsDensity), aGenerations (generations), aPopulation (aPopulationDensity)
        {
        }

        /* LIVE */
        void live ()
        {
            OptimizationGame game;

            ofstream out;
            out.open ("Plot.log");
            out << "ListPlot[{";

            for (size_t i = 0; i < aGenerations; ++i)
            {
                for (auto& individual : aPopulation)
                    game.play (&individual);

                const Individual<GeneType>& best = *max_element(aPopulation.begin(), aPopulation.end(), [](const Individual<GeneType>& a, const Individual<GeneType>& b) -> bool
                                {
                                    return a.getUtility() < b.getUtility();
                                });

                out << "{ " << i << ", " << best.getUtility () << " }";

                if (i == aGenerations - 1)
                    break;

                out << ", ";

                evolve ();
            }

            out << "}]" << endl;
            out.close ();
#if 1
            vector<size_t> results;
            for (auto individual : aPopulation)
                results.emplace_back (individual.getUtility ());
            cout << "test games:" << endl;
            cout << "\t min:" << *min_element (results.begin (), results.end ()) << endl;
            cout << "\t max:" << *max_element (results.begin (), results.end ()) << endl;
            cout << "\t avg:"
                    << ((double) accumulate (results.begin (), results.end (), (size_t) 0, plus<size_t> ())
                            / results.size ()) << endl;
#endif
        }

        /* EVOLVE */
        void evolve ();

        /* PRINT */
        template<class _GeneType>
            friend ostream& operator<< (ostream& os, const Population<_GeneType>& object);

    protected:

    private:
        size_t aPopulationDensity;
        size_t aGenerations;

        vector<Individual<GeneType>> aPopulation;

        /* Evolution roulette */
        uniform_int_distribution<size_t> sectors;
        default_random_engine roulette;

        void setEvolutionRoulette (size_t wholeUtility);
        Individual<GeneType>& getLucky ();
    };

/* PUBLIC */

/* EVOLVE */
template<typename GeneType>
    void Population<GeneType>::evolve ()
    {
        setEvolutionRoulette (
                accumulate (aPopulation.begin (), aPopulation.end (), 0,
                            [](const size_t& sum, const Individual<GeneType>& individual)
                            {   return sum + individual.getUtility();}));

        vector<Individual<GeneType>> newGeneration;

        for (size_t i = 0; i < aPopulationDensity / 2; ++i)
        {
            Individual<GeneType>& lucky1 = getLucky ();
            Individual<GeneType>& lucky2 = getLucky ();

            pair<Individual<GeneType>, Individual<GeneType>> childs = lucky1 + lucky2;
            newGeneration.emplace_back (childs.first);
            newGeneration.emplace_back (childs.second);
        }

        aPopulation.clear ();
        aPopulation.assign (newGeneration.begin (), newGeneration.end ());
        //aPopulation = newGeneration;
    }

/* PRINT */
template<typename GeneType>
    ostream& operator<< (ostream& os, const Population<GeneType>& object)
    {
        int i = 0;
        os << "Population: " << endl;

        for (auto individual : object.aPopulation)
            os << "Individual[" << i++ << "]: " << endl << individual;

        return os;
    }

/* PRIVATE */
template<typename GeneType>
    void Population<GeneType>::setEvolutionRoulette (size_t wholeUtility)
    {
        sectors = uniform_int_distribution < size_t > (0, wholeUtility);
        roulette = default_random_engine (time (0) + wholeUtility);
    }

template<typename GeneType>
    Individual<GeneType>& Population<GeneType>::getLucky ()
    {
        size_t start = 0;
        size_t lucky = sectors (roulette);

        return *find_if (aPopulation.begin (), aPopulation.end (),
                         [&start, lucky](const Individual<GeneType>& individual) -> bool
                         {
                             start += max(individual.getUtility(), (size_t)1); // Give a chance even to an individual with 0 utility
                         if (lucky <= start)
                         return true;

                         return false;
                     });
    }
