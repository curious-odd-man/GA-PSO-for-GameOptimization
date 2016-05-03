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
        size_t fieldHeight, size_t figureSize, unsigned char colorsCount, size_t paramsCount);
    ~PSO();

    void test();
    void optimize();
    void print();
    void testSolution();

#ifdef TEST
    size_t getScore()
    {   return aGbest.getUtility();}
    const vector<double>& getSolution()
    {   return aGbest.getMultipliers();}
#endif

private:

    vector<OptimizationGame> aGames;
    vector<PsoParticle> aParticles;

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

    Chronometer::TimePoint aOptimizationStart;
    Chronometer::TimePoint aOptimizationEnd;
};
