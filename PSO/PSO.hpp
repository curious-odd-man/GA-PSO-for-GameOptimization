#pragma once

using namespace std;

#include <vector>
#include "PsoParticle.hpp"
#include "Chronometer.hpp"
#include "ScientificData.hpp"

class OptimizationGame;

class PSO
{
public:
    PSO(size_t numberOfParticles, size_t numberOfIterations, size_t numberOfFinalTests, size_t fieldWidth,
        size_t fieldHeight, size_t figureSize, unsigned char colorsCount);
    ~PSO();

    void test();
    void optimize();
    void print();
    void testSolution();

    size_t getScore() { return aGbest.getUtility(); }
    const vector<double>& getSolution() { return aGbest.getMultipliers(); }

private:
    typedef struct
    {
        PsoParticle* evaluator;
        OptimizationGame* game;
    } pso_game_t;

    vector<pso_game_t> aGames;

    PsoParticle aPbest;
    PsoParticle aGbest;

    size_t aSwarmSize;
    size_t aIterationCount;
    size_t aNumberOfFinalTests;
    size_t aFieldWidth;
    size_t aFieldHeight;
    size_t aFigureSize;
    unsigned char aColorsCount;
    ScientificData aScientificData;
    // TODO: maybe add count of elements in line to remove?

    Chronometer::TimePoint aOptimizationStart;
    Chronometer::TimePoint aOptimizationEnd;

    const size_t PARAM_COUNT = 6;

};
