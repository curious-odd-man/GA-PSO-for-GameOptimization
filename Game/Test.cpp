#include "Common.hpp"
#include "Test.hpp"

size_t test_algorithm::getUtility(const vector<double>& multipliers, int hz)
{
    double sum = accumulate(multipliers.begin(), multipliers.end(), (double)0, plus<double>());
    double max = 0;
    double min = 0;

    switch (hz)
    {
        case -1:
            sum = -sum;
            max = 7;
            min = -7;
            break;
        case 0:
            sum = -fabs(sum);
            max = 0;
            min = -7;
            break;
        case 1:
            max = 7;
            min = -7;
            break;
        case 13:
            max = 7;
            min = 0;
            sum = accumulate(multipliers.begin(), multipliers.end(), (double)0, [](const double& a, const double& b)
            {   return fabs(a) + fabs(b);});
            break;
        case 66:
            // formula: 0.34x1 + 0.55x2 -0.49x3 + 0.11x4 - 0.65x5 + 0.28x6 + 0.86x7
            sum = 100
                    - (fabs(0.34 - multipliers[0]) + fabs(0.55 - multipliers[1]) + fabs(-0.49 - multipliers[2])
                            + fabs(0.11 - multipliers[3]) + fabs(-0.65 - multipliers[4]) + fabs(0.28 - multipliers[5])
                            + fabs(0.86 - multipliers[6]));
            max = 100;
            min = -1.37 - 1.55 - 1.49 - 1.11 - 1.65 - 1.28 - 1.86;
            break;
        default:
            break;
    }

    return (size_t)abs((long long)round((sum - min) / (max - min) * 1000000));
}
