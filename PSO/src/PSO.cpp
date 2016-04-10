#include <thread>
#include <future>
#include "Common.hpp"
#include "PSO.hpp"

PSO::PSO(size_t numberOfParticles, size_t numberOfIterations, size_t numberOfFinalTests, size_t fieldWidth,
         size_t fieldHeight, size_t figureSize, size_t colorsCount)
        : aSwarmSize(numberOfParticles), aIterationCount(numberOfIterations), aNumberOfFinalTests(numberOfFinalTests), aFieldWidth(
                fieldWidth), aFieldHeight(fieldHeight), aFigureSize(figureSize), aColorsCount(colorsCount)
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

void PSO::optimize()
{
    auto compare_particles = [](const pso_game_t& a, const pso_game_t& b) -> bool
    {
        return a.evaluator->getUtility() < b.evaluator->getUtility();
    };

    aOptimizationStart = Chronometer::now();

    ofstream out;
    out.open("dump.log");
    out << "ListPlot[{";

    do
    {
        // Calculate fitness value
        for (size_t i = 0; i < aGames.size(); ++i)
        {
            pso_game_t& g = aGames[i];
            g.game->play();
        }

        // find best particle
        aPbest = *(max_element(aGames.begin(), aGames.end(), compare_particles)->evaluator);
        if (aGbest < aPbest)
            aGbest = aPbest;    // update global best

        out << "{ " << aIterationCount << ", " << aGbest.getUtility() << " }";
        if (aIterationCount != 1)
            out << ", ";

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

    out << "}]" << endl;
    out.close();
}

void PSO::print()
{
    cout << DELIMITER;
    cout << "Optimization took " << Chronometer::duration_seconds(aOptimizationStart, aOptimizationEnd) << "s" << endl;
    cout << aGbest << endl;
}

void PSO::testSolution()
{
    cout << DELIMITER;
    cout << "Solution test started..." << endl;
    aTestsStart = Chronometer::now();
    vector<future<size_t>> future_results;

    for (size_t i = 0; i < aNumberOfFinalTests; ++i)
    {
        OptimizationGame g(&aGbest, aFieldWidth, aFieldHeight, aFigureSize, aColorsCount);
        future_results.push_back(async(&OptimizationGame::play, g, nullptr));
    }

    vector<size_t> results;
    for (auto& r : future_results)
        results.push_back(r.get());

    aTestsEnd = Chronometer::now();

    cout << "test games:" << endl;
    cout << "\t min:" << *min_element(results.begin(), results.end()) << endl;
    cout << "\t max:" << *max_element(results.begin(), results.end()) << endl;
    cout << "\t avg:"
            << ((double) accumulate(results.begin(), results.end(), (size_t) 0, plus<size_t>()) / results.size())
            << endl;
    cout << "\t duration: " << Chronometer::duration_seconds(aTestsStart, aTestsEnd) << "s" << endl;
}

