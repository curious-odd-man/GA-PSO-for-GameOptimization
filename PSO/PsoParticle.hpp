#pragma once
#include "UtilityEvaluator.hpp"

class PsoParticle : public UtilityEvaluator
{
public:
    PsoParticle()
            : UtilityEvaluator(), aC1(2), aC2(2)
    {
    }

    PsoParticle(const vector<double>& init, double c1 = 2.0, double c2 = 2.0);
    void move(const UtilityEvaluator& gbest, const UtilityEvaluator& pbest);

    friend ostream& operator<<(ostream& os, const PsoParticle& u);

private:
    vector<double> aVelocity;

    double aC1;
    double aC2;

    inline double getRand()
    {
        return (double) rand() / RAND_MAX;
    }
};
