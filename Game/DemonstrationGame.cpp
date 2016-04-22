#include "Common.hpp"

DemonstrationGame::DemonstrationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize,
                                     unsigned char colorsCount)
        : Game(evaluator, width, height, figureSize, colorsCount), aTurnIdx(0)
{
}

DemonstrationGame::~DemonstrationGame()
{
}

size_t DemonstrationGame::play()
{
    cout << "Demonstration game launched:" << endl;
    cout << *aUtilityEvaluator;
    cout << (aFiguresForGame.empty() ? "Figures will be generated on the fly" : "With " + to_string(aFiguresForGame.size()) + " pre-defined figures") << endl;

    Game::play();
    return aScore;
}

void DemonstrationGame::activityBeforeTurn()
{
    ++aTurnIdx;
    cout << DELIMITER << "Turn " << aTurnIdx << endl << *this << *aUtilityEvaluator;
    cin.get();
}
