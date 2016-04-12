#pragma once
using namespace std;

#include <vector>
#include <ostream>

class UtilityEvaluator
{
public:
    UtilityEvaluator()
            : aUtility(0)
    {
    }

    UtilityEvaluator(const vector<double>& init)
            : aMultipliers(init), aUtility(0)
    {
    }

    UtilityEvaluator(size_t countOfParams)
            : aMultipliers(countOfParams, (double) rand() / RAND_MAX * 2 - 1), aUtility(0)
    {
    }

    virtual ~UtilityEvaluator()
    {
    }

    virtual double evaluate(const vector<double>& params) const
    {
        double utility = 0;
        for (size_t i = 0; i < aMultipliers.size(); ++i)
            utility += aMultipliers[i] * params[i];
        return utility;
    }

    inline size_t getUtility() const
    {
        return aUtility;
    }

    inline void setUtility(size_t utility)
    {
        aUtility = utility;
    }

    inline bool operator<(const UtilityEvaluator& other)
    {
        if (aUtility < other.aUtility)
            return true;
        else
            return false;
    }

    inline const vector<double>& getMultipliers() const
    {
        return aMultipliers;
    }

    friend ostream& operator<<(ostream& os, const UtilityEvaluator& u);

protected:
    vector<double> aMultipliers;
    size_t aUtility;
};

