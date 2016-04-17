#pragma once

#include "Game.hpp"

class UtilityEvaluator;

class OptimizationGame : public Game
{
public:
    OptimizationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, unsigned char colorsCount);

    virtual ~OptimizationGame()
    {
    }

    size_t play(size_t gamesCount = DEFAULT_GAMES_COUNT);

protected:

private:

    static const size_t DEFAULT_GAMES_COUNT;

    virtual void activityBeforeTurn()
    {
        return;
    }
};

