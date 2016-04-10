#pragma once

class Game
{
public:
    Game(UtilityEvaluator& evaluator, size_t width, size_t height,  size_t figureSize, size_t colorsCount);
    Game(UtilityEvaluator& evaluator) : Game(evaluator, FIELD_WIDTH, FIELD_HEIGHT, FIGURE_SIZE, COLORS_COUNT) {};
    virtual ~Game() {};

    size_t play(UtilityEvaluator* evaluator = nullptr);      // play the game and return aScore;

    static const size_t FIELD_WIDTH;
    static const size_t FIELD_HEIGHT;
    static const size_t FIGURE_SIZE;
    static const size_t COLORS_COUNT;
    static const size_t GAMES_COUNT;

protected:
    Figure* aCurrentFigure;
    Field aField;      // current game field state, references a value in a aNextStates;
    vector<Field> aNextStates;      // list of possible next field states with calculated utility;
    UtilityEvaluator& aUtilityEvaluator;
    size_t aScore;              // game score

    size_t aFigureSize;
    size_t aColorsCount;

    virtual void activityBeforeTurn() = 0;

    inline bool gameOver()
    {
        return aNextStates.size() == 0;
    }


    friend ostream& operator<<(ostream& os, const Game& g);
};
