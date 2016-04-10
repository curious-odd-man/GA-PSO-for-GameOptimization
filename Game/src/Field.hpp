#pragma once

#include <vector>

class UtilityEvaluator;
class Figure;
class Game;

class Field
{
public:
	Field(size_t width, size_t height, UtilityEvaluator& evaluator);                                     // create new emtpy field
    Field& operator= (const Field &);

    Field(const Field& field);                                                    // use in putFigure() to generate next state,  NOTE!

	virtual ~Field();

    vector<Field>& getNextStates(const Figure& figure, vector<Field>& res) const;                      // generate all possible next field states and calculate their utility

    // compare by utility only
    bool operator<(const Field& f)
    {
        if (aUtility < f.aUtility)
            return true;
        return false;
    }

    void reset();

    size_t getRemoved() const { return aRemoved; }

 protected:
    unsigned char * aField;
    size_t aWidth;
    size_t aHeight;
    size_t aSize;
    size_t aRemoved;                                    // number of cells removed

    size_t aTwoCellsCount;
    size_t aTwoCellsOnTop;

    unsigned char * aFieldLastLineStart;
    unsigned char * aFieldLastCell;
    unsigned char * aFieldEnd;
    unsigned char * aFieldSecondLine;

    vector<double> aFieldParameters;
    vector<size_t> aColumnHeights;
    double aUtility;                                    // calculated utility for field state
    UtilityEvaluator& aUtilityEvaluator;

    void putFigure(const Figure& figure, size_t pos);        // generate next possible field states this way

    void fillRemoveMask(unsigned char* mask, bool& changed);
    void compact();
    void removeColors();                    // remove all 3 in line of same color and move blocks above down
    void calculateUtility();                // analize field and fill aUtility by using aUtilityEvaluator.evaluate();

    friend ostream& operator<<(ostream& os, const Field& f);
    void printMask(unsigned char* mask);

    void calculateFieldConstants();
};
