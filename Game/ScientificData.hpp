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

    void evaluateData()
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

            aAverageUtilities.emplace_back((double)accumulate(aAllUtilities.back().begin(), aAllUtilities.back().end(), (size_t)0, plus<size_t>()) / aAllUtilities.back().size());

            const vector<double>& multipliers = evaluators.back().getMultipliers();
            aBestMultipliers.resize(multipliers.size());
            for (size_t i = 0; i < aBestMultipliers.size(); ++i)
                aBestMultipliers[i].emplace_back(multipliers[i]);
        }
    }

    void createCharts()
    {
        evaluateData();

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

        ofstream averageUtilities;
        averageUtilities.open(aName + "_averageUtilities.log");

        for (size_t j = 0; j < aAverageUtilities.size(); ++j)
            averageUtilities << j << " " << aAverageUtilities[j] << endl;

        averageUtilities.close();
    }

private:
    vector<vector<UtilityEvaluator>> aUtilitieEvaluators;
    vector<size_t> aLocalBestUtilities;
    vector<size_t> aGlobalBestUtilities;
    vector<vector<size_t>> aAllUtilities;
    vector<vector<double>> aBestMultipliers;
    vector<double> aAverageUtilities;

    string aName;
};

