#include "Common.hpp"
#include "Test.hpp"

size_t test_game::getUtility(const vector<double>& multipliers, int hz)
{
    double sum = accumulate(multipliers.begin(), multipliers.end(), (double) 0, plus<double>()) * 100;

    switch (hz)
    {
        case -1:
            sum = 1000 + 0 - sum;
            break;
        case 0:
            sum = 1000 + 600 - fabs(sum);
            break;
        case 1:
            sum = 1000 + 0 + sum;
            break;
        case 13:
            sum = 1000
                    + accumulate(multipliers.begin(), multipliers.end(), (double) 0,
                                 [](const double& a, const double& b)
                                 {   return fabs(a) + fabs(b);}) * 100;
            break;
        default:
            break;
    }

    return (size_t) abs((long long) round(sum));
}
