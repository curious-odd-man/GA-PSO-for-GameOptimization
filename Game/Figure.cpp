#include "Common.hpp"

Figure::Figure(size_t figureSize, unsigned char colorsCount, const unsigned char * data)
        : aSize(figureSize), aColorsCount(colorsCount)
{
    aData = new unsigned char[aSize + aSize - 1];       // reserve n + n - 1
    if (data)
        memmove(aData, data, aSize);
    else
        for (size_t i = 0; i < aSize; ++i)
            aData[i] = (rand() % aColorsCount) + 1;

    // fill for next states
    memmove(aData + aSize, aData, aSize - 1);
}

Figure::Figure(const Figure& f)
{
    aSize = f.aSize;
    aColorsCount = f.aColorsCount;
    aData = new unsigned char[aSize + aSize - 1];
    memmove(aData, f.aData, aSize + aSize - 1);
}

Figure::~Figure()
{
    delete[] aData;
}

const unsigned char * Figure::begin()
{
    return aData;
}

const unsigned char * Figure::end()
{
    return aData + aSize;
}

vector<Figure> Figure::getAllStates() const
{
    vector<Figure> res;
    res.reserve(aSize);

    for (size_t i = 0; i < aSize; ++i)
        res.emplace_back(aSize, aColorsCount, aData + i);

    return res;
}

size_t Figure::getSize() const
{
    return aSize;
}

const unsigned char * Figure::getData() const
{
    return aData;
}

inline void Figure::swap(Figure & other)
{
    // Suppose swapping same figures - only data is different
    unsigned char * tmp = other.aData;
    other.aData = aData;
    aData = tmp;
}

void std::swap(Figure & first, Figure & second)
{
    first.swap(second);
}
