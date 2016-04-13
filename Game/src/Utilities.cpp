#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <future>

#include "Common.hpp"
#include "Chronometer.hpp"

ostream& operator<<(ostream& os, const Figure& f)
{
    os << "Figure@0x" << hex << &f << "; size: " << f.aSize << " countOfColors: " << (size_t) f.aColorsCount
            << "; colors: ";
    for (size_t i = 0; i < f.aSize; ++i)
        os << hex << (size_t) f.aData[i] << " ";

    os << endl;
    return os;
}

ostream& operator<<(ostream& os, const Field& f)
{
    std::cout.setf(std::ios_base::dec, std::ios_base::basefield);
    os << "Field " << f.aWidth << "x" << f.aHeight << " size " << f.aFieldSize;
    for (size_t i = 0; i < f.aFieldSize; ++i)
    {
        if (i % f.aWidth == 0)
            os << endl;
        os << +f.aField[i] << ' ';
    }
    os << endl;

    os << "Params. aRemoved: " << f.aRemoved << " aTwoCellsCount: " << f.aTwoCellsCount << " aTwoCellsOnTop: "
            << f.aTwoCellsOnTop << " aUtility: " << f.aUtility << endl;
    if (f.aFieldParameters.size())
        os << "Max column height: " << f.aFieldParameters[1] << " Average Column Height " << f.aFieldParameters[2]
                << " Column Difference " << f.aFieldParameters[3] << endl;
    else
        os << "aFieldParameters are empty" << endl;
    os << "column heights: ";
    for (auto ch : f.aColumnHeights)
        os << ch << " ";
    os << endl;
    return os;
}

ostream& operator<<(ostream& os, const Game& g)
{
    os << "Game: Score " << g.aScore << endl;
    if (g.aCurrentFigure)
    {
        os << "Figure: ";
        for (size_t i = 0; i < g.aCurrentFigure->getSize(); ++i)
            os << +(g.aCurrentFigure->getData()[i]) << " ";
        os << endl << g.aField;
    }
    return os;
}

ostream& operator<<(ostream& os, const UtilityEvaluator& u)
{
    os << "UtilityEvaluator: " << endl;
    for (auto m : u.aMultipliers)
        os << m << " ";
    os << endl;
    os << "Gained score: " << u.aUtility << endl;
    return os;
}

bool key_pressed(int* code)
{
    bool isPressed = false;
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO, &rdfs);

    select(STDIN_FILENO + 1, &rdfs, nullptr, nullptr, &tv);

    if (FD_ISSET(STDIN_FILENO, &rdfs))
    {
        int c = getchar();
        if (code != nullptr)
            *code = c;
        isPressed = 1;
    }

    return isPressed;
}

void testSolution(UtilityEvaluator& testObject, size_t count, size_t width, size_t height, size_t figureSize, size_t colorsCount, size_t gameCount)
{
	if (count == 0)
		return;
    cout << DELIMITER;
    cout << "Solution test started..." << endl;
    Chronometer::TimePoint testStart = Chronometer::now();
    vector<future<size_t>> future_results;

    for (size_t i = 0; i < count; ++i)
    {
        OptimizationGame g(&testObject, width, height, figureSize, colorsCount);
        future_results.push_back(async(&OptimizationGame::play, g, gameCount));
    }

    vector<size_t> results;
    for (auto& r : future_results)
        results.push_back(r.get());

    Chronometer::TimePoint testEnd = Chronometer::now();

    cout << "test games:" << endl;
    cout << "\t min:" << *min_element(results.begin(), results.end()) << endl;
    cout << "\t max:" << *max_element(results.begin(), results.end()) << endl;
    cout << "\t avg:"
            << ((double) accumulate(results.begin(), results.end(), (size_t) 0, plus<size_t>()) / results.size())
            << endl;
    cout << "\t duration: " << Chronometer::duration_seconds(testStart, testEnd) << "s" << endl;
}
