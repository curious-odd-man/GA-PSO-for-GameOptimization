#include "Common.hpp"

DemonstrationGame::DemonstrationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize,
                                     unsigned char colorsCount)
        : Game(evaluator, width, height, figureSize, colorsCount)
{
}

DemonstrationGame::~DemonstrationGame()
{
}

void DemonstrationGame::activityBeforeTurn()
{
    cout << *this;
    cin.get();
}