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

    UtilityEvaluator(size_t countOfParams);             // create countOfParams random multipliers
    virtual ~UtilityEvaluator()
    {
    }

    virtual double evaluate(const vector<double>& params) const;

    friend ostream& operator<<(ostream& os, const UtilityEvaluator& u);

    size_t getUtility() const;
    void setScore(size_t score);

    inline bool operator<(const UtilityEvaluator& other)
    {
        if (aUtility < other.aUtility)
            return true;
        return false;
    }

    inline const vector<double>& getMultipliers() const
    {
        return aMultipliers;
    }

protected:
    vector<double> aMultipliers;                    // multipliers for each param
    size_t aUtility;
};

