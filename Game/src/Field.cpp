#include "Common.hpp"

Field::Field (size_t width, size_t height, UtilityEvaluator* evaluator)
        : aWidth (width), aHeight (height), aSize (width * height), aRemoved (0), aTwoCellsCount (0), aTwoCellsOnTop (
                0), aColumnHeights (aWidth, 0), aUtility (0), aUtilityEvaluator (evaluator)
{
    aField = new unsigned char[aSize] ();
    calculateFieldConstants ();
    memset (aField, 0, aSize);
}

Field::Field (const Field & field)
        : aWidth (field.aWidth), aHeight (field.aHeight), aSize (field.aSize), aRemoved (field.aRemoved), aTwoCellsCount (
                field.aTwoCellsCount), aTwoCellsOnTop (field.aTwoCellsOnTop), aFieldParameters (field.aFieldParameters), aColumnHeights (
                field.aColumnHeights), aUtility (field.aUtility), aUtilityEvaluator (field.aUtilityEvaluator)
{
    aField = new unsigned char[aSize] ();
    calculateFieldConstants ();
    memmove (aField, field.aField, aSize);
}

Field& Field::operator= (const Field & field)
{
    if (this == &field)
        return *this;

    aUtilityEvaluator = field.aUtilityEvaluator;
    aWidth = field.aWidth;
    aHeight = field.aHeight;
    aUtility = field.aUtility;
    aSize = field.aSize;
    aRemoved = field.aRemoved;

    aTwoCellsCount = field.aTwoCellsCount;
    aTwoCellsOnTop = field.aTwoCellsOnTop;
    aColumnHeights.assign (field.aColumnHeights.begin (), field.aColumnHeights.end ());
    memcpy (aField, field.aField, aSize);
    aFieldParameters.assign (field.aFieldParameters.begin (), field.aFieldParameters.end ());

    return *this;
}

bool Field::operator< (const Field& f)
{
    if (aUtility < f.aUtility)
        return true;
    else
        return false;
}

Field::~Field ()
{
    delete[] aField;
}

void Field::calculateFieldConstants ()
{
    aFieldEnd = aField + aSize;
    aFieldLastLineStart = aFieldEnd - aWidth;
    aFieldLastCell = aFieldEnd - 1;
    aFieldSecondLine = aField + aWidth;
}

vector<Field>& Field::getNextStates (const Figure & figure, vector<Field>& res) const
{
    res.clear ();
    res.reserve (figure.getSize () * aWidth);
    vector<Figure> figures (figure.getAllStates ());

    // find place to put a figure
    // we can stop at third row, since we need 3 rows to put a figure
    const size_t figureEndShift = (figure.getSize () - 1) * aWidth;

    // for each column
    for (unsigned char * columnPtr = aFieldLastCell; columnPtr >= aFieldLastLineStart; --columnPtr)
    {
        // find place to put a figure
        for (unsigned char * ptr = columnPtr; ptr - figureEndShift >= aField; ptr -= aWidth)
        {
            // can place a figure?
            if (*ptr == 0)
            {
                // for each figure
                for (auto f : figures)
                {
                    // create new Field and put a current figure on that field
                    res.emplace_back (*this);
                    res.back ().putFigure (f, ptr - aField);
                }
                break;
            }
        }
    }

    return res;
}

void Field::reset ()
{
    for (unsigned char * ptr = aField; ptr < aFieldEnd; ++ptr)
        (*ptr) = 0;
}

// position is a lower end of a figure
void Field::putFigure (const Figure & figure, size_t pos)
{
    for (size_t i = 0; i < figure.getSize (); ++i)
        aField[pos - aWidth * i] = figure.getData ()[i];
    removeColors ();
}

void Field::fillRemoveMask (unsigned char* mask, bool& changed)
{
    // Test and mark for deletion
    auto TnM_for_deletion = [this](const unsigned char * ptr, unsigned char &lastColor,
            unsigned char * removeMask, bool &field_changed, unsigned char &colorRepeats,
            int didx)
    {
        if (*ptr != 0)
        {
            if (lastColor == *ptr)
            {
                unsigned char* ptr_in_mask = removeMask + (ptr - aField);
                ++colorRepeats;
                if (colorRepeats == 3)
                {
                    *ptr_in_mask = *ptr;
                    *(ptr_in_mask + didx) = *(ptr + didx);
                    *(ptr_in_mask + (didx << 1)) = *(ptr + (didx << 1));
                    field_changed = true;
                }
                else if (colorRepeats > 3)
                *ptr_in_mask = *ptr;
                else if (colorRepeats == 2)
                {
                    ++aTwoCellsCount;
                    if (::in_range(aField, aFieldLastCell, ptr + (didx << 1)) && *(ptr + (didx << 1)) == 0)
                    aTwoCellsOnTop++;
                    else if (::in_range(aField, aFieldLastCell, ptr - didx) && *(ptr - didx) == 0)
                    aTwoCellsOnTop++;
                }
            }
            else
            {
                colorRepeats = 1;
                lastColor = *ptr;
            }
        }
        else
        {
            lastColor = 0;
            colorRepeats = 0;
        }
    };

    changed = false;
    memset (mask, 0, aSize);

    unsigned char lastColor = 0;
    unsigned char colorRepeats = 0;

    // search horizontal lines
    for (unsigned char * start = aField; start < aFieldEnd; start += aWidth)
    {
        lastColor = 0;
        colorRepeats = 0;
        for (unsigned char* ptr = start; ptr < start + aWidth; ++ptr)
            TnM_for_deletion (ptr, lastColor, mask, changed, colorRepeats, -1);
    }

    // search vertical lines from the end
    for (unsigned char * columnPtr = aFieldLastCell; columnPtr >= aFieldLastLineStart; --columnPtr)
    {
        lastColor = 0;
        colorRepeats = 0;
        for (unsigned char * ptr = columnPtr; ptr >= aField; ptr -= aWidth)
            TnM_for_deletion (ptr, lastColor, mask, changed, colorRepeats, (int) aWidth);
    }

    if (aWidth < 3 || aHeight < 3)  // can 3 in line be on diagonale?
        return;

    // search up-left - down-right diagonales
    //                 Field:
    // field start-> . . . . . . . . * . <- endPtr
    //   startPtr->  * . . . . . . . . .
    //               . . . . . . . . . .          
    //               . . * . . . . . . . <- diagEndPtr   
    lastColor = 0;
    colorRepeats = 0;
    unsigned char* startPtr = aFieldEnd - 3 * aWidth;
    unsigned char* endPtr = aFieldSecondLine - 2;
    unsigned char* diagEndPtr = aFieldEnd - (aWidth - 2);

    for (unsigned char * ptr = startPtr; ptr != endPtr;)
    {
        TnM_for_deletion (ptr, lastColor, mask, changed, colorRepeats, -((int) aWidth + 1));

        if (ptr != diagEndPtr)      // reached end of diagonale
            ptr += aWidth + 1;
        else
        {
            // if startPtr at top field line
            if (startPtr - aWidth < aField)
                ++startPtr;     // move start one tile right
            else
                startPtr -= aWidth;     // move start one tile up

            ptr = startPtr;

            // isn't diagEndPtr at last field line?
            if (diagEndPtr > aFieldLastLineStart && diagEndPtr < aFieldLastCell)
                ++diagEndPtr;               // yes, move diag end one tile right
            else
                diagEndPtr -= aWidth;       // no, move diag end one tile up

            lastColor = 0;
            colorRepeats = 0;
        }
    }

    // search up-right - down-left diagonales
    //                 Field:
    //               . . * . . . . . . . <- startPtr
    //               . . . . . . . . . .
    //diagEndPtr ->  * . . . . . . . . * <- endPtr
    //               . . . . . . . . . . 
    lastColor = 0;
    colorRepeats = 0;
    startPtr = aField + 2;
    endPtr = aFieldLastLineStart - 1;
    diagEndPtr = aFieldSecondLine + aWidth;

    for (unsigned char * ptr = startPtr; ptr != endPtr;)
    {
        TnM_for_deletion (ptr, lastColor, mask, changed, colorRepeats, -((int) aWidth - 1));

        if (ptr != diagEndPtr)      // reached end of diagonale
            ptr += aWidth - 1;
        else
        {
            // if startPtr at top field line
            if (startPtr < aFieldSecondLine - 1)
                ++startPtr;     // move start one tile right
            else
                startPtr += aWidth;     // move start one tile down

            ptr = startPtr;

            // isn't diagEndPtr at last field line?
            if (diagEndPtr < aFieldLastLineStart)
                diagEndPtr += aWidth;       // no, move diag end one tile down
            else
                ++diagEndPtr;               // yes, move diag end one tile right

            lastColor = 0;
            colorRepeats = 0;
        }
    }
}

void Field::compact ()
{
    for (unsigned char * columnPtr = aFieldLastCell; columnPtr >= aFieldLastLineStart; --columnPtr)
    {
        unsigned char * lastEmpty = nullptr;
        for (unsigned char * ptr = columnPtr; ptr >= aField; ptr -= aWidth)
        {
            if (lastEmpty == nullptr && *ptr == 0)
                lastEmpty = ptr;
            else if (lastEmpty != nullptr && *ptr != 0)
            {
                *lastEmpty = *ptr;
                lastEmpty -= aWidth;
                *ptr = 0;
            }
        }
    }
}

void Field::removeColors ()
{
    unsigned char* removeMask = new unsigned char[aSize];
    bool fieldChanged = false;     // flag if field has changed
    aRemoved = 0;

    // find all "3 of same color in line" and mark for removal
    do
    {
        aTwoCellsCount = 0;
        aTwoCellsOnTop = 0;
        fillRemoveMask (removeMask, fieldChanged);
        if (!fieldChanged)
            break;

        for (size_t i = 0; i < aSize; ++i)
            if (removeMask[i] > 0)
            {
                ++aRemoved;
                aField[i] = 0;
            }

        compact ();

        // repeat from start unless nothing was removed
    } while (fieldChanged);

    for (unsigned char * columnPtr = aFieldLastCell; columnPtr >= aFieldLastLineStart; --columnPtr)
    {
        size_t columnIdx = columnPtr - aFieldLastLineStart;
        aColumnHeights[columnIdx] = 0;
        for (unsigned char * ptr = columnPtr; ptr >= aField && *ptr != 0; ptr -= aWidth)
            ++aColumnHeights[columnIdx];
    }

    calculateUtility ();

    delete[] removeMask;
}

void Field::calculateUtility ()
{
    size_t sum = 0;
    size_t last = 0;
    size_t avgColumnDiff = 0;
    for (auto ch : aColumnHeights)
    {
        sum += ch;
        if (last != 0)
            avgColumnDiff += abs ((int) (last - ch));
        last = ch;
    }

    aFieldParameters.assign (
        { (double) aRemoved,                                                       // count of removed cells
                (double) *max_element (aColumnHeights.begin (), aColumnHeights.end ()),     // max column height
                (double) sum / aWidth,                                                   // average column height
                (double) avgColumnDiff,                                                // column difference absolute sum
                (double) aTwoCellsCount,                                       // Count of 2 cells with one color in row
                (double) aTwoCellsOnTop       // Count of previous cells that are on top (can be remove with next turn)
            });
    aUtility = aUtilityEvaluator->evaluate (aFieldParameters);
}

void Field::printMask (unsigned char* mask)
{
    for (size_t i = 0; i < aSize; ++i)
    {
        if (i % aWidth == 0)
            cout << endl;
        cout << +mask[i] << ' ';
    }
    cout << endl;
}
