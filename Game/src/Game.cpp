#include "Common.hpp"

const size_t Game::FIELD_WIDTH = 7;
const size_t Game::FIELD_HEIGHT = 14;
const size_t Game::FIGURE_SIZE = 3;
const size_t Game::COLORS_COUNT = 7;
const size_t Game::GAMES_COUNT = 10;

Game::Game (UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, size_t colorsCount)
        : aCurrentFigure (nullptr), aField (width, height, evaluator), aUtilityEvaluator (evaluator), aScore (0), aFigureSize (
                figureSize), aColorsCount (colorsCount)
{
}

size_t Game::play (UtilityEvaluator* evaluator)
{
    vector<size_t> game_scores (GAMES_COUNT, 0);

    for (size_t i = 0; i < GAMES_COUNT; ++i)
    {
        aScore = 0;
        aField.reset ();
        aNextStates.clear ();

        if (evaluator)
        {
            aUtilityEvaluator = evaluator;
            aField.setEvaluator(evaluator);
        }

        while (true)
        {
            Figure fig (3, 7);
            aCurrentFigure = &fig;
            aField.getNextStates (fig, aNextStates);

            activityBeforeTurn ();
            if (gameOver ())
                break;

            aField = *max_element (aNextStates.begin (), aNextStates.end ());
            aScore += aField.getRemoved ();
        }

        game_scores[i] = aScore;
    }

    aScore = accumulate (game_scores.begin (), game_scores.end (), 0ULL, [](size_t a, size_t b)
    {   return a + b;}) / game_scores.size ();
    aUtilityEvaluator->setScore (aScore);
    return aScore;
}

