#pragma once

class Game
{
public:
    Game(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, size_t colorsCount);
    Game(UtilityEvaluator* evaluator);
    Game();

    virtual ~Game()
    {
    }

    size_t play();

    inline void setEvaluator(UtilityEvaluator* evaluator)
    {
        aUtilityEvaluator = evaluator;
    }

protected:
    Figure* aCurrentFigure;
    Field aField;      // current game field state, references a value in a aNextStates;
    vector<Field> aNextStates;      // list of possible next field states with calculated utility;
    UtilityEvaluator* aUtilityEvaluator;
    size_t aScore;              // game score

    size_t aFigureSize;
    size_t aColorsCount;

    virtual void activityBeforeTurn() = 0;

    inline bool gameOver()
    {
        return aNextStates.size() == 0;
    }

    friend ostream& operator<<(ostream& os, const Game& g);

    const size_t DEFAULT_FIELD_WIDTH = 7;
    const size_t DEFAULT_FIELD_HEIGHT = 14;
    const size_t DEFAULT_FIGURE_SIZE = 3;
    const size_t DEFAULT_COLORS_COUNT = 7;
};
