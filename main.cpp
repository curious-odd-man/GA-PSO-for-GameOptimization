#include <iomanip>
#include "Common.hpp"

#include "PSO.hpp"
#include "GA.hpp"



//#define TEST

// Algorithm defaults
const size_t DEFAULT_ITERATION_COUNT = 50;
const size_t DEFAULT_ITERATION_STRENGTH = 50;
const size_t DEFAULT_SOLUTION_TESTS_COUNT = 50;

const size_t DEFAULT_COEFFICIENT_COUNT = 6;             // FIXME: args?
const double DEFAULT_MINIMAL_COEFFICIENT_VALUE = -1.0;  // FIXME: args?
const double DEFAULT_MAXIMAL_COEFFICIENT_VALUE = 1.0;   // FIXME: args?

// Game defaults
const size_t DEFAULT_FIELD_WIDTH = 7;
const size_t DEFAULT_FIELD_HEIGHT = 14;
const size_t DEFAULT_FIGURE_SIZE = 3;
const size_t DEFAULT_COLORS_COUNT = 7;

void printHelp()
{
    cout << "Algorithm settings:" << endl;
    cout << "\t-a S: choose algorithm (GA, PSO, BOTH by default)" << endl;
    cout << "\t-i N: set number of iterations [1.. " << ULLONG_MAX << "] " << DEFAULT_ITERATION_COUNT << " by default"
            << endl;
    cout << "\t-s N: set iteration strength [1.. " << ULLONG_MAX << "] " << DEFAULT_ITERATION_STRENGTH << " by default"
            << endl;
    cout << "\t-t N: set number of solution tests [1.. " << ULLONG_MAX << "]" << DEFAULT_SOLUTION_TESTS_COUNT
            << " by default" << endl;
    cout << "Game settings:" << endl;
    cout << "\t-w N: set field width [1.. " << ULLONG_MAX << "] " << DEFAULT_FIELD_WIDTH << " by default" << endl;
    cout << "\t-h N: set field height [1.. " << ULLONG_MAX << "] " << DEFAULT_FIELD_HEIGHT << " by default" << endl;
    cout << "\t-f N: set figure size [1.. " << ULLONG_MAX << "] " << DEFAULT_FIGURE_SIZE << " by default" << endl;
    cout << "\t-c N: set count of colors [1.. " << ULLONG_MAX << "] " << DEFAULT_COLORS_COUNT << " by default" << endl;
    cout << "Debug settings:" << endl;
    cout << "\t-d B: set debug on/off [true.. false] 0 by default" << endl;
}

int main(int argc, char** argv)
{
    /* Set up DEFAULTS parameters */
    string algorithm("BOTH");
    size_t iterations = DEFAULT_ITERATION_COUNT;
    size_t strength = DEFAULT_ITERATION_STRENGTH;
    size_t numberOfSolutionTests = DEFAULT_SOLUTION_TESTS_COUNT;
    size_t fieldWidth = DEFAULT_FIELD_WIDTH;
    size_t fieldHeight = DEFAULT_FIELD_HEIGHT;
    size_t figureSize = DEFAULT_FIGURE_SIZE;
    unsigned char colorsCount = DEFAULT_COLORS_COUNT;
    bool debug = false;
    size_t coefficientCount = DEFAULT_COEFFICIENT_COUNT;
    double coefficientMin = DEFAULT_MINIMAL_COEFFICIENT_VALUE;
    double coefficientMax = DEFAULT_MAXIMAL_COEFFICIENT_VALUE;

    for (int i = 1; i < argc; ++i)
    {
        string param(argv[i]);

        transform(param.begin(), param.end(), param.begin(), ::tolower);

        if (param == "--help")
        {
            printHelp();
            return 0;
        }

        if (param[0] != '-')
            continue;   // skip malformed parameter

        stringstream value(argv[++i]);      // take parameter value

        switch (param[1])
        {
            case 'a':
                value >> algorithm;
                break;
            case 'c':
                value >> colorsCount;
                break;
            case 'd':
                value >> boolalpha >> debug;
                break;
            case 'f':
                value >> figureSize;
                break;
            case 'h':
                value >> fieldHeight;
                break;
            case 'i':
                value >> iterations;
                break;
            case 's':
                value >> strength;
                break;
            case 't':
                value >> numberOfSolutionTests;
                break;
            case 'w':
                value >> fieldWidth;
                break;
            default:
                break;
        }
    }
#ifdef TEST
    typedef struct
    {
        int idx;
        string algo;
        size_t score;
        vector<double> solution;
    } test_struct;

    vector<test_struct> tests;

    for (int i = 0; i < 20; ++i)
#endif
    {

        if (algorithm == "PSO" || algorithm == "BOTH")
        {
            PSO p(strength, iterations, numberOfSolutionTests, fieldWidth, fieldHeight, figureSize, colorsCount);
            if (debug)
                p.test();
            else
            {
                p.optimize();
                p.print();
                p.testSolution();
#ifdef TEST
                tests.push_back({i, "PSO", p.getScore(), p.getSolution()});
#endif
            }
        }

        if (algorithm == "GA" || algorithm == "BOTH")
        {
            Population<double> p(coefficientCount, coefficientMin, coefficientMax, strength, iterations);

            if (debug)
                p.test(coefficientCount, coefficientMin, coefficientMax, fieldWidth, fieldHeight, figureSize, colorsCount);
            else
            {
                p.live(numberOfSolutionTests, fieldWidth, fieldHeight, figureSize, colorsCount);
#ifdef TEST
                tests.push_back({ i, "GA", p.getScore(), p.getSolution() });
#endif
            }
        }
    }
#ifdef TEST
    ofstream out;
    out.open("solution_coef_comp.log");
    for (auto& t : tests)
    {
        out << setw(3) << t.idx << " " << setw(5) << t.algo << ": " << setw(5) << t.score << " ";
        for (auto k : t.solution)
            out << setw(12) << k << " ";
        out << endl;
    }
    out.close();
#endif
}
