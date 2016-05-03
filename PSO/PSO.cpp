#include "Common.hpp"
#include "PSO.hpp"
#include "Test.hpp"
#include "ScientificData.hpp"

PSO::PSO(size_t numberOfParticles, size_t numberOfIterations, size_t numberOfFinalTests, size_t fieldWidth,
         size_t fieldHeight, size_t figureSize, unsigned char colorsCount, size_t countOfParams)
        : aSwarmSize(numberOfParticles), aIterationCount(numberOfIterations), aNumberOfFinalTests(numberOfFinalTests), aFieldWidth(
                fieldWidth), aFieldHeight(fieldHeight), aFigureSize(figureSize), aColorsCount(colorsCount), aScientificData(
                "PSO")
{
    srand((unsigned int)time(NULL));

    default_random_engine generator;
    uniform_real_distribution<double> distribution(-1.0, 1.0);

    // PSO initialization
    aParticles.reserve(aSwarmSize);
    for (size_t i = 0; i < aSwarmSize; ++i)
    {
        vector<double> init;
        for (size_t j = 0; j < countOfParams; ++j)
            init.emplace_back(distribution(generator));

        // TODO: change when game is reusable
        aParticles.emplace_back(init);
        aGames.emplace_back(&aParticles.back(), aFieldWidth, aFieldHeight, aFigureSize, aColorsCount);
    }
}

PSO::~PSO()
{
}

void PSO::test()
{
    int test[] =
        { -1, 0, 1, 13, 66 };

    for (int j = 0; j < 5; ++j)
    {
        ScientificData log(string("PSO_test") + to_string(j));
        vector<OptimizationGame> games;
        vector<PsoParticle> particles;

        for (auto& p : aParticles)
            particles.emplace_back(p);
        for (auto& g : aGames)
            games.emplace_back(g);

        PsoParticle gbest;
        size_t iterations = aIterationCount;

        do
        {
            for (auto& p : particles)
                p.setUtility(test_algorithm::getUtility(p.getMultipliers(), test[j]));

            PsoParticle pbest = *(max_element(particles.begin(), particles.end()));
            if (gbest < pbest)
                gbest = pbest;    // update global best

            log.addStatisticalData(vector < UtilityEvaluator >(particles.begin(), particles.end()));

            // move particles
            for (auto& p : particles)
                p.move(gbest, pbest);
        } while (--iterations > 0);
        cout << "Test " << test[j] << " " << gbest << endl;
        log.createCharts();
    }
}

void PSO::optimize()
{
    aOptimizationStart = Chronometer::now();

    do
    {
        // Calculate fitness value
#ifdef USE_PARALEL_OPTIMIZATION
        vector < future < size_t >> future_results;
        for (size_t i = 0; i < aGames.size(); ++i)
            future_results.push_back(
                    async(&OptimizationGame::play, &aGames[i], OptimizationGame::DEFAULT_GAMES_COUNT));

        for (auto& r : future_results)
            r.get();
#else
        for (auto& g : aGames)
            g.play();
#endif

        aPbest = *(max_element(aParticles.begin(), aParticles.end()));
        if (aGbest < aPbest)
            aGbest = aPbest;

        // TODO: don't create vector to copy data
        aScientificData.addStatisticalData(vector < UtilityEvaluator >(aParticles.begin(), aParticles.end()));

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
        for (auto& p : aParticles)
            p.move(aGbest, aPbest);

    } while (--aIterationCount > 0);

    aOptimizationEnd = Chronometer::now();
}

void PSO::testSolution()
{
    ::testSolution("PSO", aGbest, aNumberOfFinalTests, aFieldWidth, aFieldHeight, aFigureSize, aColorsCount, 1);
}

void PSO::print()
{
    cout << DELIMITER;
    cout << "Optimization took " << Chronometer::duration(aOptimizationStart, aOptimizationEnd) << endl;
    cout << aGbest << endl << "creating charts now..." << endl;
    aScientificData.createCharts();
    cout << "creating charts done!" << endl;
}

