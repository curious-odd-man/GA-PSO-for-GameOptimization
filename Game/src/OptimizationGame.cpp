#include "Common.hpp"


OptimizationGame::OptimizationGame(UtilityEvaluator & evaluator, size_t width, size_t height, size_t figureSize, size_t colorsCount)
    : Game(evaluator, width, height, figureSize, colorsCount)
{
}

void OptimizationGame::activityBeforeTurn()
{
    // dump data as necessary
}
