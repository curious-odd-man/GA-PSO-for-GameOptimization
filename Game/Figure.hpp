#pragma once

class Figure
{
public:
    Figure(size_t figureSize, unsigned char colorsCount, const unsigned char * data = nullptr);
    Figure(const Figure& f);
    ~Figure();

    // iterator simulation

    const unsigned char * begin();
    const unsigned char * end();

    vector<Figure> getAllStates() const; // get all possible variations of this figure (variations count is equal with aSize;

    size_t getSize() const;                     // for the Field; (put figure on field and generate new Field states);

    const unsigned char * getData() const;            // to put figure on a field

    friend ostream& operator<<(ostream& os, const Figure& f);

private:
    size_t aSize;
    unsigned char aColorsCount;
    unsigned char *aData;
};

