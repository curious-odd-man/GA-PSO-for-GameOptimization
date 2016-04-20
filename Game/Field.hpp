#pragma once

#include <vector>

class UtilityEvaluator;
class Figure;
class Game;

class Field
{
public:
    Field(size_t width, size_t height, UtilityEvaluator* evaluator);
    Field(const Field& field);

    virtual ~Field();

    /* OPERATORS */
    Field& operator=(const Field &);
    bool operator<(const Field& f);

    /* METHODS */
    vector<Field>& getNextStates(const Figure& figure, vector<Field>& res) const;

    void reset();

    inline void setEvaluator(UtilityEvaluator* evaluator)
    {
        aUtilityEvaluator = evaluator;
    }

    size_t getRemoved() const
    {
        return aRemoved;
    }

protected:

    unsigned char* aField;
    size_t aWidth;
    size_t aHeight;
    size_t aFieldSize;
    size_t aRemoved;                                    // number of cells removed

    size_t aTwoCellsCount;
    size_t aTwoCellsOnTop;

    unsigned char* aFieldLeftBottomCorner;
    unsigned char* aFieldRightBottomCorner;
    unsigned char* aFieldEnd;
    unsigned char* aFieldSecondLine;

    vector<double> aFieldParameters;
    vector<size_t> aColumnHeights;
    double aUtility;                                    // calculated utility for field state
    UtilityEvaluator* aUtilityEvaluator;

    void putFigure(const Figure& figure, size_t pos);        // generate next possible field states this way

    bool markRemovable(unsigned char* mask);
    void compact();
    void removeColors();                    // remove all 3 in line of same color and move blocks above down
    void calculateUtility();                // analize field and fill aUtility by using aUtilityEvaluator.evaluate();

    friend ostream& operator<<(ostream& os, const Field& f);
    void printMask(unsigned char* mask);

    void calculateFieldConstants();

    const unsigned char EMPTY_CELL = 0;
    const unsigned char REMOVABLE_SIZE = 3;
};
