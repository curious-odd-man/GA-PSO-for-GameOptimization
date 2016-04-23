#include "Common.hpp"

Field::Field(size_t width, size_t height, UtilityEvaluator* evaluator)
        : aWidth(width), aHeight(height), aFieldSize(width * height), aRemoved(0), aTwoCellsCount(0), aTwoCellsOnTop(0), aColumnHeights(
                aWidth, 0), aUtility(0), aUtilityEvaluator(evaluator)
{
    aField = new unsigned char[aFieldSize]();
    calculateFieldConstants();
    memset(aField, 0, aFieldSize);
}

Field::Field(const Field & field)
        : aWidth(field.aWidth), aHeight(field.aHeight), aFieldSize(field.aFieldSize), aRemoved(field.aRemoved), aTwoCellsCount(
                field.aTwoCellsCount), aTwoCellsOnTop(field.aTwoCellsOnTop), aFieldParameters(field.aFieldParameters), aColumnHeights(
                field.aColumnHeights), aUtility(field.aUtility), aUtilityEvaluator(field.aUtilityEvaluator)
{
    aField = new unsigned char[aFieldSize]();
    calculateFieldConstants();
    memmove(aField, field.aField, aFieldSize);
}

Field& Field::operator=(const Field & field)
{
    if (this == &field)
        return *this;

    aUtilityEvaluator = field.aUtilityEvaluator;
    aWidth = field.aWidth;
    aHeight = field.aHeight;
    aUtility = field.aUtility;
    aFieldSize = field.aFieldSize;
    aRemoved = field.aRemoved;

    aTwoCellsCount = field.aTwoCellsCount;
    aTwoCellsOnTop = field.aTwoCellsOnTop;
    aColumnHeights.assign(field.aColumnHeights.begin(), field.aColumnHeights.end());
    memcpy(aField, field.aField, aFieldSize);
    aFieldParameters.assign(field.aFieldParameters.begin(), field.aFieldParameters.end());

    return *this;
}

bool Field::operator<(const Field& f) const
{
    return aUtility < f.aUtility;
}

Field::~Field()
{
    delete[] aField;
}

void Field::calculateFieldConstants()
{
    aFieldEnd = aField + aFieldSize;
    aFieldLeftBottomCorner = aFieldEnd - aWidth;
    aFieldRightBottomCorner = aFieldEnd - 1;
    aFieldSecondLine = aField + aWidth;
}

vector<Field>& Field::getNextStates(const Figure& figure, vector<Field>& nextStates) const
{
    nextStates.clear();
    nextStates.reserve(figure.getSize() * aWidth);
    vector<Figure> figureStates(figure.getAllStates());

    // find place to put a figure
    // we can stop at third row, since we need 3 rows to put a figure
    const unsigned char* effectiveColumnHeight = aField + (figure.getSize() - 1) * aWidth;

    // for each column
    for (unsigned char* columnPtr = aFieldRightBottomCorner; columnPtr >= aFieldLeftBottomCorner; --columnPtr)
    {
        // find place to put a figure
        for (unsigned char* cellPtr = columnPtr; cellPtr >= effectiveColumnHeight; cellPtr -= aWidth)
        {
            // can place a figure?
            if (*cellPtr == 0)
            {
                // for each figure state
                for (auto f : figureStates)
                {
                    // create new Field and put a current figure on that field
                    nextStates.emplace_back(*this);
                    nextStates.back().putFigure(f, cellPtr - aField);
                }
                break;
            }
        }
    }

    return nextStates;
}

void Field::reset()
{
    memset(aField, 0, aFieldSize);
}

// position is a lower end of a figure
void Field::putFigure(const Figure& figure, size_t pos)
{
    const unsigned char* figureData = figure.getData();
    unsigned char* figureStart = aField + pos;
    const unsigned char* figureEnd = figureStart - aWidth * figure.getSize();
    for (unsigned char* cellPtr = figureStart; cellPtr > figureEnd; cellPtr -= aWidth)
        *cellPtr = *figureData++;
#ifdef TEST_PARAMS
    cout << DELIMITER << "Field before removing colors (NOTE! Parameters are invalid!!)" << endl << *this;
#endif
    removeColors(figure.getSize());
#ifdef TEST_PARAMS
    cout << "Field with recalculated params " << endl << *this << DELIMITER << endl;
#endif
}

bool Field::markRemovable(unsigned char* mask)
{
    bool changed = false;
    unsigned char lastColor = EMPTY_CELL;
    unsigned char colorRepeats = 0;

#ifdef TEST_PARAMS_TWO_IN_LINE
    cout << "Mark removable called" << endl;
#endif
    auto isReachable =
            [this](const unsigned char* start) -> bool
            {
                for (const unsigned char* cellPtr = start + aWidth; cellPtr <= start + REMOVABLE_SIZE * aWidth; cellPtr += aWidth)
                if (cellPtr > aFieldRightBottomCorner || *cellPtr != EMPTY_CELL)
                return true;
                return false;
            };

    // Test and mark for deletion
    auto testAndMark =
            [this, &mask, &changed, &lastColor, &colorRepeats, &isReachable](const unsigned char* start, int didx)
            {
                if (*start != EMPTY_CELL)
                {
                    if (lastColor == *start)
                    {
                        unsigned char* ptr_in_mask = mask + (start - aField);
                        ++colorRepeats;
                        if (colorRepeats == REMOVABLE_SIZE)
                        {
                            const int endIndex = didx * REMOVABLE_SIZE;
                            for (int i = 0; i != endIndex; i += didx)
                            *(ptr_in_mask + i) = *(start + i);
                            changed = true;
                        }
                        else if (colorRepeats > REMOVABLE_SIZE)
                        *ptr_in_mask = *start;
                        else if (colorRepeats > 1)
                        {
#ifdef TEST_PARAMS_TWO_IN_LINE
            const size_t offset = cellPtr - aField;
            const size_t x = offset % aWidth;
            const size_t y = offset / aWidth;
            cout << "two of one color: " << x << " " << y << endl;
#endif
            const unsigned char* preCell = start - didx;
            const unsigned char* postCell = start + didx * 2;

            if (::in_range(aField, aFieldRightBottomCorner, postCell) && *postCell == EMPTY_CELL && isReachable(postCell))
            ++aTwoCellsOnTop;
            else if (::in_range(aField, aFieldRightBottomCorner, preCell) && *preCell == EMPTY_CELL && isReachable(preCell))
            ++aTwoCellsOnTop;
            else
            ++aTwoCellsCount;
        }
    }
    else
    {
        colorRepeats = 1;
        lastColor = *start;
    }
}
else
{
    lastColor = EMPTY_CELL;
    colorRepeats = 0;
}
}   ;

    aTwoCellsCount = 0;
    aTwoCellsOnTop = 0;
    memset(mask, EMPTY_CELL, aFieldSize);

#ifdef TEST_PARAMS_TWO_IN_LINE
    cout << " Horizontal search " << endl;
#endif

    // for each line
    for (unsigned char * lineStart = aField; lineStart < aFieldEnd; lineStart += aWidth)
    {
        lastColor = EMPTY_CELL;
        colorRepeats = 0;
        const unsigned char* lineEnd = lineStart + aWidth;
        // for each cell in a line
        for (unsigned char* start = lineStart; start < lineEnd; ++start)
            testAndMark(start, -1);
    }

#ifdef TEST_PARAMS_TWO_IN_LINE
    cout << " Vertical search " << endl;
#endif

    // for each column
    for (unsigned char* columnPtr = aFieldRightBottomCorner; columnPtr >= aFieldLeftBottomCorner; --columnPtr)
    {
        lastColor = EMPTY_CELL;
        colorRepeats = 0;
        // for each cell in column
        for (unsigned char* start = columnPtr; start >= aField; start -= aWidth)
            testAndMark(start, (int)aWidth);
    }

    if (aWidth < REMOVABLE_SIZE || aHeight < REMOVABLE_SIZE)  // can 3 in line be on diagonale?
        return changed;

    // search up-left - down-right diagonals
    //                 Field:
    // field start-> . . . . . . . . * . <- endPtr
    //   startPtr->  * . . . . . . . . .
    //               . . . . . . . . . .          
    //               . . * . . . . . . . <- diagEndPtr
    lastColor = EMPTY_CELL;
    colorRepeats = 0;
    unsigned char* startPtr = aFieldEnd - REMOVABLE_SIZE * aWidth;
    unsigned char* endPtr = aFieldSecondLine - REMOVABLE_SIZE + 1;
    unsigned char* diagEndPtr = aFieldEnd - aWidth + REMOVABLE_SIZE - 1;

#ifdef TEST_PARAMS_TWO_IN_LINE
    cout << " ul-dr search " << endl;
#endif

    for (unsigned char * start = startPtr; start != endPtr;)
    {
        testAndMark(start, -((int)aWidth + 1));

        if (start != diagEndPtr)
            start += aWidth + 1;
        else
        {
            if (startPtr - aWidth < aField)
                ++startPtr;
            else
                startPtr -= aWidth;

            start = startPtr;

            if (diagEndPtr > aFieldLeftBottomCorner && diagEndPtr < aFieldRightBottomCorner)
                ++diagEndPtr;
            else
                diagEndPtr -= aWidth;

            lastColor = EMPTY_CELL;
            colorRepeats = 0;
        }
    }

    // search up-right - down-left diagonales
    //                 Field:
    //               . . * . . . . . . . <- startPtr
    //               . . . . . . . . . .
    //diagEndPtr ->  * . . . . . . . . * <- endPtr
    //               . . . . . . . . . . 
    lastColor = EMPTY_CELL;
    colorRepeats = 0;
    startPtr = aField + REMOVABLE_SIZE - 1;
    endPtr = aFieldLeftBottomCorner - 1;
    diagEndPtr = aFieldSecondLine + aWidth;

#ifdef TEST_PARAMS_TWO_IN_LINE
    cout << " ur-dl search " << endl;
#endif

    for (unsigned char* ptr = startPtr; ptr != endPtr;)
    {
        testAndMark(ptr, -((int)aWidth - 1));

        if (ptr != diagEndPtr)
            ptr += aWidth - 1;
        else
        {
            if (startPtr < aFieldSecondLine - 1)
                ++startPtr;
            else
                startPtr += aWidth;

            ptr = startPtr;

            if (diagEndPtr < aFieldLeftBottomCorner)
                diagEndPtr += aWidth;
            else
                ++diagEndPtr;

            lastColor = EMPTY_CELL;
            colorRepeats = 0;
        }
    }

    return changed;
}

void Field::compact()
{
    // for each column
    for (unsigned char* columnPtr = aFieldRightBottomCorner; columnPtr >= aFieldLeftBottomCorner; --columnPtr)
    {
        unsigned char* lastEmptyCell = nullptr;
        // for each cell in column
        for (unsigned char* cellPtr = columnPtr; cellPtr >= aField; cellPtr -= aWidth)
        {
            if (lastEmptyCell == nullptr && *cellPtr == EMPTY_CELL)
                lastEmptyCell = cellPtr;
            else if (lastEmptyCell != nullptr && *cellPtr != EMPTY_CELL)
            {
                *lastEmptyCell = *cellPtr;
                lastEmptyCell -= aWidth;
                *cellPtr = EMPTY_CELL;
            }
        }
    }
}

void Field::removeColors(size_t figureSize)
{
    unsigned char* removeMask = new unsigned char[aFieldSize];
    aRemoved = 0;

    // find all "3 of same color in line" and mark for removal
    while (markRemovable(removeMask))
    {
        for (size_t i = 0; i < aFieldSize; ++i)
        {
            if (removeMask[i] != EMPTY_CELL)
            {
                ++aRemoved;
                aField[i] = EMPTY_CELL;
            }
        }
        compact();
    }

    // recalculate column heights
    for (unsigned char* columnPtr = aFieldRightBottomCorner; columnPtr >= aFieldLeftBottomCorner; --columnPtr)
    {
        size_t columnIdx = columnPtr - aFieldLeftBottomCorner;
        aColumnHeights[columnIdx] = 0;
        for (unsigned char* cellPtr = columnPtr; cellPtr >= aField && *cellPtr != EMPTY_CELL; cellPtr -= aWidth)
            ++aColumnHeights[columnIdx];
    }

    calculateUtility(figureSize);

    delete[] removeMask;
}

void Field::calculateUtility(size_t figureSize)
{
    size_t columnHeightsSum = 0;
    size_t lastColumnHeight = -1;
    size_t avgColumnDiff = 0;
    size_t placesForFigure = 0;
    for (auto columnHeight : aColumnHeights)
    {
        columnHeightsSum += columnHeight;
        if (lastColumnHeight != (size_t) - 1)
            avgColumnDiff += abs((int)(lastColumnHeight - columnHeight));
        lastColumnHeight = columnHeight;
        if (columnHeight <= aHeight - figureSize)
            ++placesForFigure;
    }

    aFieldParameters.assign(
        { (double)aRemoved,                                                            // count of removed cells
                (double)*max_element(aColumnHeights.begin(), aColumnHeights.end()),    // max column height
                (double)columnHeightsSum / aWidth,                                     // average column height
                (double)avgColumnDiff,                                                 // column difference absolute sum
                (double)aTwoCellsCount,                                        // Count of 2 cells with one color in row
                (double)aTwoCellsOnTop,       // Count of previous cells that are on top (can be remove with next turn)
                (double)placesForFigure      // number of next states
            });

    aUtility = aUtilityEvaluator->evaluate(aFieldParameters);
}

void Field::printMask(unsigned char* mask)
{
    for (size_t i = 0; i < aFieldSize; ++i)
    {
        if (i % aWidth == 0)
            cout << endl;
        cout << +mask[i] << ' ';
    }
    cout << endl;
}
