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

    UtilityEvaluator(istream& in)
            : aUtility(0)
    {
        while (in)
        {
            aMultipliers.emplace_back(0.0);
            in >> aMultipliers.back();
        }
        aMultipliers.pop_back();        // last element is invalid
    }

    UtilityEvaluator(const vector<double>& init)
            : aMultipliers(init), aUtility(0)
    {
    }

    UtilityEvaluator(size_t countOfParams)
            : aMultipliers(countOfParams), aUtility(0)
    {
        for (auto& multiplier : aMultipliers)
            multiplier = (double)rand() / RAND_MAX * 2 - 1;
    }

    virtual ~UtilityEvaluator()
    {
    }

    inline UtilityEvaluator& operator=(const UtilityEvaluator& other)
    {
        aUtility = other.aUtility;
        aMultipliers.assign(other.aMultipliers.begin(), other.aMultipliers.end());
        return *this;
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
        return aUtility < other.aUtility;
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

