#pragma once
#include "Common.hpp"

#include "Test.hpp"
#include "Individual.hpp"
#include "OptimizationGame.hpp"
#include "ScientificData.hpp"

template<typename GeneType>
class Population
{
public:
    Population(size_t geneCount, GeneType minValue, GeneType maxValue, size_t populationsDensity, size_t generations)
            : aPopulationDensity(populationsDensity + 1 /* for the Best */), aGenerations(generations)
    {
        for (size_t i = 0; i < aPopulationDensity; ++i)
            aPopulation.emplace_back(Individual<GeneType>(geneCount, minValue, maxValue));
    }

    void test(size_t geneCount, GeneType minValue, GeneType maxValue, size_t fieldWidth, size_t fieldHeight,
              size_t figureSize, unsigned char colorsCount)
    {
        OptimizationGame game(nullptr, fieldWidth, fieldHeight, figureSize, colorsCount);

        int test[] =
            { -1, 0, 1, 13 };

        for (int j = 0; j < 4; ++j)
        {
            ScientificData log(string("GA_test") + to_string(j));
            aPopulation.clear();
            aPopulation = vector<Individual<GeneType>>(aPopulationDensity, Individual<GeneType>(geneCount, minValue, maxValue));

            for (size_t i = 0; i < aGenerations; ++i)
            {
                for (auto& individual : aPopulation)
                    individual.setUtility(test_game::getUtility(individual.getMultipliers(), test[j]));

                log.addStatisticalData(vector<UtilityEvaluator>(aPopulation.begin(), aPopulation.end()));

                if (i == aGenerations - 1)
                    break;

                evolve();
            }

            vector<size_t> results;
            for (auto individual : aPopulation)
                results.emplace_back(individual.getUtility());

            cout << "Test " << test[j] << " " << *max_element(results.begin(), results.end()) << endl;
            cout << aPopulation.back() << endl;

            log.createCharts();
        }
    }

    /* LIVE */
    void live(size_t numberOfSolutionTests, size_t fieldWidth, size_t fieldHeight, size_t figureSize,
              unsigned char colorsCount)
    {
        ScientificData science("GA");

        OptimizationGame game(nullptr, fieldWidth, fieldHeight, figureSize, colorsCount);

        for (size_t i = 0; i < aGenerations; ++i)
        {
            for (auto& individual : aPopulation)
            {
                game.setEvaluator(&individual);
                game.play();
            }

            science.addStatisticalData(vector<UtilityEvaluator>(aPopulation.begin(), aPopulation.end()));

            if (i == aGenerations - 1)
                break;

            evolve();
        }

        vector<size_t> results;
        for (auto individual : aPopulation)
            results.emplace_back(individual.getUtility());
        cout << "test games:" << endl;
        cout << "\t min:" << *min_element(results.begin(), results.end()) << endl;
        cout << "\t max:" << *max_element(results.begin(), results.end()) << endl;
        cout << "\t avg:"
                << ((double) accumulate(results.begin(), results.end(), (size_t) 0, plus<size_t>()) / results.size())
                << endl;

        science.createCharts();
    }

    /* EVOLVE */
    void evolve();

    /* PRINT */
    template<class _GeneType>
    friend ostream& operator<<(ostream& os, const Population<_GeneType>& object);

protected:

private:
    size_t aPopulationDensity;
    size_t aGenerations;

    vector<Individual<GeneType>> aPopulation;

    /* Evolution roulette */
    uniform_int_distribution<size_t> sectors;
    default_random_engine roulette;

    void setEvolutionRoulette(size_t wholeUtility);
    Individual<GeneType>& getLucky();
};

/* PUBLIC */

/* EVOLVE */
template<typename GeneType>
void Population<GeneType>::evolve()
{
    setEvolutionRoulette(
            accumulate(aPopulation.begin(), aPopulation.end(), (size_t) 0,
                       [](const size_t& sum, const Individual<GeneType>& individual)
                       {   return sum + individual.getUtility();}));

    vector<Individual<GeneType>> newGeneration;

    Individual<GeneType>& theBest = *max_element(
            aPopulation.begin(), aPopulation.end(),
            [](const Individual<GeneType>& first, const Individual<GeneType>& second)
            {   return first.getUtility() < second.getUtility();});

    for (size_t i = 0; i < aPopulationDensity / 2; ++i)
    {
        Individual<GeneType>& lucky1 = getLucky();
        Individual<GeneType>& lucky2 = getLucky();

        pair<Individual<GeneType>, Individual<GeneType>> childs = lucky1 + lucky2;
        newGeneration.emplace_back(childs.first);
        newGeneration.emplace_back(childs.second);
    }

    newGeneration.emplace_back(theBest);

    aPopulation.clear();
    aPopulation.assign(newGeneration.begin(), newGeneration.end());
}

/* PRINT */
template<typename GeneType>
ostream& operator<<(ostream& os, const Population<GeneType>& object)
{
    int i = 0;
    os << "Population: " << endl;

    for (auto individual : object.aPopulation)
        os << "Individual[" << i++ << "]: " << endl << individual;

    return os;
}

/* PRIVATE */
template<typename GeneType>
void Population<GeneType>::setEvolutionRoulette(size_t wholeUtility)
{
    sectors = uniform_int_distribution < size_t > (0, wholeUtility);
    roulette = default_random_engine((unsigned) (time(0) + wholeUtility));
}

template<typename GeneType>
Individual<GeneType>& Population<GeneType>::getLucky()
{
    size_t start = 0;
    size_t lucky = sectors(roulette);

    return *find_if(aPopulation.begin(), aPopulation.end(),
                    [&start, lucky](const Individual<GeneType>& individual) -> bool
                    {
                        start += max(individual.getUtility(), (size_t)1); // Give a chance even to an individual with 0 utility
                    if (lucky <= start)
                    return true;

                    return false;
                });
}
