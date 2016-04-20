#include "Common.hpp"

Game::Game(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, unsigned char colorsCount)
        : aCurrentFigure(nullptr), aField(width, height, evaluator), aUtilityEvaluator(evaluator), aScore(0), aFigureSize(
                figureSize), aColorsCount(colorsCount)
{
}

size_t Game::play()
{
    aScore = 0;
    aField.reset();
    aNextStates.clear();

#ifdef TEST_PARAMS
    cout << DELIMITER << endl;
    cout << DELIMITER << endl;
    cout << "NEW GAME STARTED" << endl;
#endif

    while (true)
    {
        aCurrentFigure = new Figure(aFigureSize, aColorsCount);
#ifdef TEST_PARAMS
        cout << DELIMITER << "New turn!" << endl << "current field is " << aField << endl;
#endif
        aField.getNextStates(*aCurrentFigure, aNextStates);

        activityBeforeTurn();
        if (gameOver())
            break;

        aField = *max_element(aNextStates.begin(), aNextStates.end());
        aScore += aField.getRemoved();
        delete aCurrentFigure;
    }

    return aScore;

}

