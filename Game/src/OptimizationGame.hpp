#pragma once

#include "Game.hpp"

class UtilityEvaluator;

class OptimizationGame : public Game
{
public:
    OptimizationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, size_t colorsCount);
    OptimizationGame(UtilityEvaluator* evaluator) :Game(evaluator) {};
    OptimizationGame() {};

    virtual ~OptimizationGame() {};

protected:

private:
    virtual void activityBeforeTurn();
};

