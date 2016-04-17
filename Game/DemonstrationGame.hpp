#pragma once
#include "Game.hpp"

class UtilityEvaluator;

class DemonstrationGame : public Game
{
public:
    DemonstrationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, unsigned char colorsCount);
    virtual ~DemonstrationGame();

private:
    void activityBeforeTurn();
};

