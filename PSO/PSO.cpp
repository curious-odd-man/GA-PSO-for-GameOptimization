#include <thread>
#include <future>
#include "Common.hpp"
#include "PSO.hpp"
#include "Test.hpp"
#include "ScientificData.hpp"

PSO::PSO(size_t numberOfParticles, size_t numberOfIterations, size_t numberOfFinalTests, size_t fieldWidth,
    size_t fieldHeight, size_t figureSize, unsigned char colorsCount)
    : aSwarmSize(numberOfParticles), aIterationCount(numberOfIterations), aNumberOfFinalTests(numberOfFinalTests), aFieldWidth(
        fieldWidth), aFieldHeight(fieldHeight), aFigureSize(figureSize), aColorsCount(colorsCount), aScientificData("PSO")
{
    srand((unsigned int) time(NULL));
    cout << DELIMITER;
    cout << "PSO otimization created with " << aSwarmSize << " particles and " << aIterationCount << " iterations"
            << endl;
    cout << "\t Field width is " << aFieldWidth << ", field height is " << aFieldHeight << endl;
    cout << "\t Figure size is " << aFigureSize << endl;
    cout << "\t " << aColorsCount << " diffrent colors" << endl;
    cout << "\t Solution will be tested on " << aNumberOfFinalTests << " games." << endl;

    default_random_engine generator;
    uniform_real_distribution<double> distribution(-1.0, 1.0);

    // PSO initialization
    for (size_t i = 0; i < aSwarmSize; ++i)
    {
        vector<double> init =
            { distribution(generator), distribution(generator), distribution(generator), distribution(generator),
                    distribution(generator), distribution(generator) };

        // TODO: change when game is reusable
        PsoParticle * p = new PsoParticle(init);
        aGames.push_back(
            { p, new OptimizationGame(p, aFieldWidth, aFieldHeight, aFigureSize, aColorsCount) });
    }
}

PSO::~PSO()
{
    for (auto p : aGames)
    {
        delete p.game;
        delete p.evaluator;
    }
}

void PSO::test()
{
    auto compare_particles = [](const pso_game_t& a, const pso_game_t& b) -> bool
    {
        return a.evaluator->getUtility() < b.evaluator->getUtility();
    };

    int test[] =
        { -1, 0, 1, 13 };

    for (int j = 0; j < 4; ++j)
    {
        ScientificData log(string("PSO_test") + to_string(j));
        vector<pso_game_t> games;
        for (auto& g : aGames)
            games.push_back(
                { new PsoParticle(*g.evaluator), new OptimizationGame(*g.game) });
        PsoParticle gbest;
        int test[] =
            { -1, 0, 1, 13 };
        size_t iterations = 100;

        do
        {
            for (size_t i = 0; i < games.size(); ++i)
                games[i].evaluator->setUtility(test_game::getUtility(games[i].evaluator->getMultipliers(), test[j]));

            PsoParticle pbest = *(max_element(games.begin(), games.end(), compare_particles)->evaluator);
            if (gbest < pbest)
                gbest = pbest;    // update global best
            vector<UtilityEvaluator> allEvaluators;
            for (size_t i = 0; i < games.size(); ++i)
                allEvaluators.push_back(*games[i].evaluator);

            log.addStatisticalData(allEvaluators);

            // move particles
            for (auto g : games)
                g.evaluator->move(gbest, pbest);
        } while (--iterations > 0);
        cout << "Test " << test[j] << " " << gbest << endl;
        for (auto p : games)
        {
            delete p.game;
            delete p.evaluator;
        }
        log.createCharts();
    }

}

void PSO::optimize()
{
    auto compare_particles = [](const pso_game_t& a, const pso_game_t& b) -> bool
    {
        return a.evaluator->getUtility() < b.evaluator->getUtility();
    };

    aOptimizationStart = Chronometer::now();


    do
    {
        // Calculate fitness value
        for (size_t i = 0; i < aGames.size(); ++i)
        {
            pso_game_t& g = aGames[i];
            g.game->play();
        }

        aPbest = *(max_element(aGames.begin(), aGames.end(), compare_particles)->evaluator);
        if (aGbest < aPbest)
            aGbest = aPbest;    // update global best

        vector<UtilityEvaluator> allEvaluators;
        for (size_t i = 0; i < aGames.size(); ++i)
            allEvaluators.push_back(*aGames[i].evaluator);

        aScientificData.addStatisticalData(allEvaluators);

        // TODO: this should be moved into g->play();
        int c = 0;
        while (key_pressed(&c))
            ;

        if (c)
        {
            switch (c)
            {
                case 'P':
                case 'p':
                    cout << "Optimization status: " << endl;
                    cout << "\t Iterations left: " << aIterationCount << endl;
                    cout << "\t Current gbest: " << endl;
                    cout << aGbest << endl;
                    cout << "\t Current pbest: " << endl;
                    cout << aPbest << endl;
                    break;

                case 'T':
                case 't':
                    cout << "Optimization terminated by user" << endl;
                    aOptimizationEnd = Chronometer::now();
                    return;
                    break;

                default:
                    break;
            }
        }
        // move particles 
        for (auto g : aGames)
            g.evaluator->move(aGbest, aPbest);

    } while (--aIterationCount > 0);

    aOptimizationEnd = Chronometer::now();
}

void PSO::testSolution()
{
    ::testSolution(aGbest, aNumberOfFinalTests, aFieldWidth, aFieldHeight, aFigureSize, aColorsCount, 1);
}

void PSO::print()
{
    cout << DELIMITER;
    cout << "Optimization took " << Chronometer::duration_seconds(aOptimizationStart, aOptimizationEnd) << "s" << endl;
    cout << aGbest << endl << "creating charts now..." << endl;
    aScientificData.createCharts();
    cout << "creating charts done!" << endl;
}

