#include <Common.hpp>
#include "PsoParticle.hpp"

const double INERTIA = 1.2;

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
    bool reduce = false;
    double reduction = 1;

    for (size_t i = 0; i < aMultipliers.size(); ++i)
    {
        aVelocity[i] = aVelocity[i] * INERTIA + aC1 * getRand() * (gbest.getMultipliers()[i] - aMultipliers[i])
                + aC2 * getRand() * (pbest.getMultipliers()[i] - aMultipliers[i]);
        if (aVelocity[i] > 0.1)
        {
            reduction = min(reduction, 0.1 / aVelocity[i]);
            reduce = true;
        }
    }

    for (size_t i = 0; i < aMultipliers.size(); ++i)
    {
        if (reduce)
            aVelocity[i] *= reduction;
        aMultipliers[i] += aVelocity[i];
        if (aMultipliers[i] > 1.0)
            aMultipliers[i] = 1.0;
        else if (aMultipliers[i] < -1.0)
            aMultipliers[i] = -1.0;
    }
}

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
