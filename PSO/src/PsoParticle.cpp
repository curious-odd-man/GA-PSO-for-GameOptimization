#include <Common.hpp>
#include "PsoParticle.hpp"

PsoParticle::PsoParticle(size_t countOfParams, double c1, double c2)
        : UtilityEvaluator(countOfParams), aC1(c1), aC2(c2)
{
    while (countOfParams-- > 0)
        aVelocity.push_back((double) rand() / RAND_MAX * 0.1 - 0.05);
}

PsoParticle::PsoParticle(const vector<double>& init, double c1, double c2)
        : UtilityEvaluator(init), aC1(c1), aC2(c2)
{
    size_t countOfParams = init.size();
    while (countOfParams-- > 0)
        aVelocity.push_back((double) rand() / RAND_MAX * 0.1 - 0.05);
}

void PsoParticle::move(const UtilityEvaluator & gbest, const UtilityEvaluator & pbest)
{
    aUtility = 0;
    double sumOfSquares = 0;

    for (size_t i = 0; i < aMultipliers.size(); ++i)
    {
        aVelocity[i] += aC1 * getRand() * (gbest.getMultipliers()[i] - aMultipliers[i])
                + aC2 * getRand() * (pbest.getMultipliers()[i] - aMultipliers[i]);
        sumOfSquares += aVelocity[i] * aVelocity[i];
    }

    double vectorReductionMultiplier = 1;
    bool reduce = sumOfSquares > 0.01;
    if (reduce)   // if length of a vector is greater than 0.1, reduce all vector components by vectorReductionMultilier
        vectorReductionMultiplier = 0.1 / sqrt(sumOfSquares);

    for (size_t i = 0; i < aMultipliers.size(); ++i)
    {
        if (reduce)
            aVelocity[i] *= vectorReductionMultiplier;
        aMultipliers[i] += aVelocity[i];
        if (aMultipliers[i] > 1.0)
            aMultipliers[i] = 1.0;
        else if (aMultipliers[i] < -1.0)
            aMultipliers[i] = -1.0;
    }
}
