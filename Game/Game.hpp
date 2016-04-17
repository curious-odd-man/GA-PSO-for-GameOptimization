#pragma once

class Game
{
public:
    Game(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, unsigned char colorsCount);

    virtual ~Game()
    {
    }

    size_t play();

    inline void setEvaluator(UtilityEvaluator* evaluator)
    {
        aUtilityEvaluator = evaluator;
        aField.setEvaluator(evaluator);
    }

protected:
    Figure* aCurrentFigure;
    Field aField;      // current game field state, references a value in a aNextStates;
    vector<Field> aNextStates;      // list of possible next field states with calculated utility;
    UtilityEvaluator* aUtilityEvaluator;
    size_t aScore;              // game score

    size_t aFigureSize;
    unsigned char aColorsCount;

    virtual void activityBeforeTurn() = 0;

    inline bool gameOver()
    {
        return aNextStates.size() == 0;
    }

    friend ostream& operator<<(ostream& os, const Game& g);
};
