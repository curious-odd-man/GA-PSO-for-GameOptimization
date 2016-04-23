#pragma once

#include "Common.hpp"

class ScientificData
{
public:
    ScientificData(string name)
            : aName(string("LOGS/") + name)
    {
    }

    virtual ~ScientificData()
    {
    }

    inline void addStatisticalData(const vector<UtilityEvaluator>& evaluators)
    {
        aUtilitieEvaluators.emplace_back();
        for (auto evaluator : evaluators)
            aUtilitieEvaluators.back().emplace_back(evaluator);
    }

    void evaluate_data()
    {
        size_t globalBest = 0;
        for (auto& evaluators : aUtilitieEvaluators)
        {
            sort(evaluators.begin(), evaluators.end(), [](const UtilityEvaluator& first, const UtilityEvaluator& second)
            {   return first.getUtility() < second.getUtility();});

            if (globalBest < evaluators.back().getUtility())
                globalBest = evaluators.back().getUtility();
            aLocalBestUtilities.push_back(evaluators.back().getUtility());
            aGlobalBestUtilities.push_back(globalBest);
            aAllUtilities.emplace_back();
            for (auto& evaluator : evaluators)
                aAllUtilities.back().emplace_back(evaluator.getUtility());

            const vector<double>& multipliers = evaluators.back().getMultipliers();
            aBestMultipliers.resize(multipliers.size());
            for (size_t i = 0; i < aBestMultipliers.size(); ++i)
                aBestMultipliers[i].emplace_back(multipliers[i]);
        }
    }

    void createCharts()
    {
        evaluate_data();

        ofstream allUtilities;
        allUtilities.open(aName + "_allUtilities.log");

        // AllUtilities
        for (size_t j = 0; j < aAllUtilities.size(); ++j)
            for (size_t i = 0; i < aAllUtilities[j].size(); ++i)
                allUtilities << j << " " << i << " " << aAllUtilities[j][i] << endl;
        allUtilities.close();

        ofstream twoBestUtilities;
        twoBestUtilities.open(aName + "_twoBestUtilities.log");
        // Best Utilities
        for (size_t i = 0; i < aLocalBestUtilities.size(); ++i)
            twoBestUtilities << i << " " << aLocalBestUtilities[i] << endl << i << " " << aGlobalBestUtilities[i]
                    << endl;
        twoBestUtilities.close();

        ofstream bestMultipliers;
        bestMultipliers.open(aName + "_bestMultipliers.log");
        // BestMultipliers
        for (size_t j = 0; j < aBestMultipliers.size(); ++j)
        {
            for (size_t i = 0; i < aBestMultipliers[j].size(); ++i)
                bestMultipliers << i << " " << aBestMultipliers[j][i] << endl;
        }

        bestMultipliers.close();
    }

private:
    vector<vector<UtilityEvaluator>> aUtilitieEvaluators;
    vector<size_t> aLocalBestUtilities;
    vector<size_t> aGlobalBestUtilities;
    vector<vector<size_t>> aAllUtilities;
    vector<vector<double>> aBestMultipliers;

    string aName;
};

