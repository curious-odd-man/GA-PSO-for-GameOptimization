#include "Common.hpp"



#include "UtilityEvaluator.hpp"

UtilityEvaluator::UtilityEvaluator(size_t countOfParams)
    : aUtility(0)
{
    while (countOfParams-- > 0)
        aMultipliers.push_back((double)rand() / RAND_MAX * 2 - 1);
}

double UtilityEvaluator::evaluate(const vector<double>& params) const
{
    if (params.size() != aMultipliers.size())
        raise(SIGABRT);

    double utility = 0;
    for (size_t i = 0; i < aMultipliers.size(); ++i)
        utility += aMultipliers[i] * params[i];
    return utility;
}

size_t UtilityEvaluator::getUtility() const
{
    return aUtility;
}

void UtilityEvaluator::setScore(size_t score)
{
    aUtility = score;
}
