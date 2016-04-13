#include "Common.hpp"
#include "PSO.hpp"

ostream& operator<<(ostream& os, const PsoParticle& u)
{
    os << "PSOUtilityEvaluator: " << endl << "\tMultipliers: ";
    for (auto m : u.aMultipliers)
        os << m << " ";
    os << endl << "\tVector: ";
    for (auto m : u.aVelocity)
        os << m << " ";
    os << endl;
    os << "\tGained score: " << u.aUtility << endl;
    return os;
}

void printHelp()
{
    cout << "Particle swarm optimization arguments" << endl;
    cout << "each parameter has a value separated by space" << endl;
    cout << "\t-w N: set field width to N (7 by default)" << endl;
    cout << "\t-h N: set field height to N (14 by default)" << endl;
    cout << "\t-f N: set figure size to N (3 by default)" << endl;
    cout << "\t-c N: set count of colors to N (7 by default)" << endl;
    cout << "\t-s N: set swarm size to N (100 by default)" << endl;
    cout << "\t-i N: set number of iterations to N (100 by default)" << endl;
    cout << "\t-t N: set solution tests count to N (100 by default)" << endl;
}

int main(int argc, char** argv)
{
    size_t swarmSize = 2;
    size_t iterations = 2;
    size_t numberOfSolutionTests = 0;
    size_t fieldWidth = 7;
    size_t fieldHeight = 14;
    size_t figureSize = 3;
    size_t colorsCount = 7;

    for (int i = 1; i < argc; ++i)
    {
        string param(stringstream(argv[i]).str());
        transform(param.begin(), param.end(), param.begin(), ::tolower);
        if (param[0] == '?')
        {
            printHelp();
            return 0;
        }
        if (param[0] != '-')
            continue;   // skip malformatted parameter
        stringstream value(argv[++i]);      // take parameter value
        switch (param[1])
        {
            case 'w':
                value >> fieldWidth;
                break;
            case 'h':
                value >> fieldHeight;
                break;
            case 'f':
                value >> figureSize;
                break;
            case 'c':
                value >> colorsCount;
                break;
            case 'i':
                value >> iterations;
                break;
            case 's':
                value >> swarmSize;
                break;
            case 't':
                value >> numberOfSolutionTests;
                break;
            default:
                break;
        }
    }

    {
    	PSO p(swarmSize, iterations, numberOfSolutionTests, fieldWidth, fieldHeight, figureSize, colorsCount);
    	p.optimize(-1);
    	p.print();
    }

#if 0
    {
		PSO p(swarmSize, iterations, numberOfSolutionTests, fieldWidth, fieldHeight, figureSize, colorsCount);
		p.optimize(0);
		p.print();
	}
    {
		PSO p(swarmSize, iterations, numberOfSolutionTests, fieldWidth, fieldHeight, figureSize, colorsCount);
		p.optimize(1);
		p.print();
	}
    {
		PSO p(swarmSize, iterations, numberOfSolutionTests, fieldWidth, fieldHeight, figureSize, colorsCount);
		p.optimize(13);
		p.print();
	}
#endif
}
