#pragma once

#include "Common.hpp"

class ScientificData
{
public:
	ScientificData(string name) : aName(name) {};
    virtual ~ScientificData() {};

    inline void addStatisticalData(const vector<UtilityEvaluator>& evaluators)
    {
        aUtilitieEvaluators.emplace_back(evaluators.size());
        for (auto evaluator : evaluators)
            aUtilitieEvaluators.back().emplace_back(evaluator);
    }

    void evaluate_data()
    {
        for (auto& evaluators : aUtilitieEvaluators)
        {
            sort(evaluators.begin(), evaluators.end(), [](const UtilityEvaluator& first, const UtilityEvaluator& second) { return first.getUtility() < second.getUtility(); });
            aBestUtilities.push_back(evaluators.back().getUtility());
            const vector<double>& multipliers = evaluators.back().getMultipliers();
            aBestMultipliers.resize(multipliers.size());
            for (size_t i = 0; i < aBestMultipliers.size(); ++i)
                aBestMultipliers[i].emplace_back(multipliers[i]);
            a2ndBestUtilities.push_back(evaluators[evaluators.size() - 2].getUtility());
        }
    }

	void createCharts()
	{
        evaluate_data();
		ofstream out;
		out.open(aName.c_str());
		// Utilities
		out << "Show[ListPlot[{";
		for (size_t i = 0; i < aBestUtilities.size(); ++i)
		    out << "{ " << i << ", " << aBestUtilities[i] << " }" << (i != aBestUtilities.size() - 1 ? ", " : "");
        out << "}]" << endl << ", ListPlot[{";

        for (size_t i = 0; i < a2ndBestUtilities.size(); ++i)
            out << "{ " << i << ", " << a2ndBestUtilities[i] << " }" << (i != a2ndBestUtilities.size() - 1 ? ", " : "");
        out << "}]]" << endl << "ListPlot[{";

		// BestMultipliers
		for (size_t j = 0; j < aBestMultipliers.size(); ++j)
		{
            out << "{";
			for (size_t i = 0; i < aBestMultipliers[j].size(); ++i)
				out << "{ " << i << ", " << aBestMultipliers[j][i] << " }" << (i != aBestMultipliers[j].size() - 1 ? ", " : "");
            out << "}" << (j != aBestMultipliers.size() - 1 ? ", " : "");
		}
        out << "}, PlotLegends -> {";
        for (size_t j = 0; j < aBestMultipliers.size(); ++j)
            out << j << (j != aBestMultipliers.size() - 1 ? ", " : "");
        out << "}]" << endl;

		out.close();
	}

private:
	vector<vector<UtilityEvaluator>> aUtilitieEvaluators;
    vector<size_t> aBestUtilities;
    vector<size_t> a2ndBestUtilities;
    vector<vector<double>> aBestMultipliers;

	string aName;
};

