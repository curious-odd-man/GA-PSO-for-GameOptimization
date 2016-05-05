#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "Common.hpp"
#include "Chronometer.hpp"

bool colorsSupported(size_t colorsCount)
{
    if (colorsCount <= 7)
        return true;
    else
        return false;
}

#ifdef _WIN32

#define clearColorText resetColor()

string resetColor()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    return "";
}

string getFigureColorText(int color)
{
    static const unsigned colors[] = {
        BACKGROUND_INTENSITY | BACKGROUND_BLUE,
        BACKGROUND_INTENSITY | BACKGROUND_GREEN,
        BACKGROUND_INTENSITY | BACKGROUND_RED,
        BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN,
        BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED,
        BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED,
        BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE,
        BACKGROUND_RED,
    };

    if (color > 0 && color < (sizeof(colors) / sizeof(unsigned)))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[color]);
        return "";
    }
    else
        return to_string(color);

    
}


constexpr auto UPPER_LEFT_CORNER =  "\xC9";
constexpr auto LOWER_LEFT_CORNER =  "\xC8";
constexpr auto UPPER_RIGHT_CORNER = "\xBB";
constexpr auto LOWER_RIGHT_CORNER = "\xBC";
constexpr auto HORIZONTAL_BORDER =  "\xCD";
constexpr auto VERTICAL_BORDER =    "\xBA";

#else

constexpr auto clearColorText = "\033[0m";

string getFigureColorText(int color)
{
    return string("\033[" + to_string(100 + color) + "m");
}


constexpr auto UPPER_LEFT_CORNER = "\u2554"; 
constexpr auto LOWER_LEFT_CORNER = "\u255A"; 
constexpr auto UPPER_RIGHT_CORNER = "\u2557";
constexpr auto LOWER_RIGHT_CORNER = "\u255D";
constexpr auto HORIZONTAL_BORDER = "\u2550"; 
constexpr auto VERTICAL_BORDER = "\u2551";   
#endif

ostream& operator<<(ostream& os, const Figure& f)
{
    os << "Figure: ";
    for (size_t i = 0; i < f.aSize; ++i)
    {
        if (&os == &cout && colorsSupported(f.aColorsCount))
        {
            os << getFigureColorText(f.aData[i]) << "  ";
            os << clearColorText;
        }
        else
            os << to_string(f.aData[i]) << " ";
    }

    return os;
}

ostream& operator<<(ostream& os, const Field& f)
{
    
    std::cout.setf(std::ios_base::dec, std::ios_base::basefield);
    os << "Field " << f.aWidth << "x" << f.aHeight << " size " << f.aFieldSize;
    os << endl << UPPER_LEFT_CORNER;
    for (size_t i = 0; i < f.aWidth * 2; ++i)
        os << HORIZONTAL_BORDER;
    os << UPPER_RIGHT_CORNER << endl << VERTICAL_BORDER;

    for (size_t i = 0; i < f.aFieldSize; ++i)
    {
        if (i && i % f.aWidth == 0)
            os << VERTICAL_BORDER << endl << VERTICAL_BORDER;

        if (f.aField[i] == 0)
            os << "  ";
        else if (&os == &cout && colorsSupported(f.aField[i]))
        {
            os << getFigureColorText(f.aField[i]) << "  ";
            os << clearColorText;
        }
        else 
            os << to_string(f.aField[i]) << " ";
    }

    os << VERTICAL_BORDER << endl << LOWER_LEFT_CORNER;
    for (size_t i = 0; i < f.aWidth * 2; ++i)
        os << HORIZONTAL_BORDER;
    os << LOWER_RIGHT_CORNER << endl;

    if (f.aFieldParameters.size())
    {
        os << "Params:" << endl;
        os << "\tRemoved:                        " << f.aFieldParameters[0] << endl;
        os << "\tMax column height:              " << f.aFieldParameters[1] << endl;
        os << "\tAverage column height:          " << f.aFieldParameters[2] << endl;
        os << "\tColumn difference:              " << f.aFieldParameters[3] << endl;
        os << "\ttwo cells of same color:        " << f.aFieldParameters[4] << endl;
        os << "\ttwo cells of same color on top: " << f.aFieldParameters[5] << endl;
        os << "\tPlaces for new figure:          " << f.aFieldParameters[6] << endl;
    }
    else
        os << "aFieldParameters are empty" << endl;

    os << "column heights: ";
    for (auto ch : f.aColumnHeights)
        os << ch << " ";
    os << endl << "Utility: " << f.aUtility;
    os << endl;
    return os;
}

ostream& operator<<(ostream& os, const DemonstrationGame& g)
{
    os << "Game: Score " << g.aScore << endl;
    if (g.aFiguresHistory.size())
        os << g.aFiguresHistory.back() << endl;
    if (!g.gameOver())
        os << *max_element(g.aNextStates.begin(), g.aNextStates.end());
    else
        os << endl << "\t\t\tGame Over!\n" << endl;
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
#ifndef _WIN32
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
#else
    bool pressed = false;
    while (_kbhit())
    {
        *code = _getch();
        pressed = true;
    }

    return pressed;
#endif
}

void testSolution(string algo, UtilityEvaluator& testObject, size_t count, size_t width, size_t height,
                  size_t figureSize, unsigned char colorsCount, size_t gameCount)
{
    if (count == 0)
        return;
    cout << DELIMITER;
    cout << "Solution test started for " << algo << "..." << endl;

    Chronometer::TimePoint testStart = Chronometer::now();
    vector < future < size_t >> future_results;
    vector<OptimizationGame> games;
    vector<UtilityEvaluator> evaluators(count, testObject);
    for (size_t i = 0; i < count; ++i)
        games.emplace_back(&evaluators[i], width, height, figureSize, colorsCount);

    for (size_t i = 0; i < count; ++i)
        future_results.push_back(async(&OptimizationGame::play, &games[i], gameCount));

    vector < size_t > results;
    for (auto& r : future_results)
        results.push_back(r.get());

    size_t worst = *min_element(results.begin(), results.end());
    size_t best = *max_element(results.begin(), results.end());

    Chronometer::TimePoint testEnd = Chronometer::now();
    cout << "test games:" << endl;
    cout << testObject << endl;
    cout << "\t min:" << worst << endl;
    cout << "\t max:" << best << endl;
    cout << "\t avg:"
            << ((double)accumulate(results.begin(), results.end(), (size_t)0, plus<size_t>()) / results.size()) << endl;
    cout << "\t duration: " << Chronometer::duration(testStart, testEnd) << endl;

    min_element(games.begin(), games.end())->dumpFiguresHistory("figures" + to_string(best - worst) + ".log");
}
