#include "Common.hpp"

Game::Game (UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, size_t colorsCount)
        : aCurrentFigure (nullptr), aField (width, height, evaluator), aUtilityEvaluator (evaluator), aScore (0), aFigureSize (
                figureSize), aColorsCount (colorsCount)
{
}

size_t Game::play ()
{
    aScore = 0;
    aField.reset ();
    aNextStates.clear ();

    while (true)
    {
        aCurrentFigure = new Figure (aFigureSize, aColorsCount);
        aField.getNextStates (*aCurrentFigure, aNextStates);

        activityBeforeTurn ();
        if (gameOver ())
            break;

        aField = *max_element (aNextStates.begin (), aNextStates.end ());
        aScore += aField.getRemoved ();
        delete aCurrentFigure;
    }

    return aScore;

}

