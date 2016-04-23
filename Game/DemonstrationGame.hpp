#pragma once
#include "Game.hpp"

class UtilityEvaluator;

class DemonstrationGame : public Game
{
public:
    DemonstrationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize,
                      unsigned char colorsCount);
    virtual ~DemonstrationGame();
    virtual size_t play();

    friend ostream& operator<<(ostream& os, const DemonstrationGame& g);

private:
    size_t aTurnIdx;
    size_t aStopIdx;

    void activityBeforeTurn();
    void printAllStatesUtilities();
    void printAllStateOneByOne();
};

