#pragma once

#include "Common.hpp"

class ScientificData
{
public:
	ScientificData(string name) : aName(name) {};
	virtual ~ScientificData();

	inline void addStatisticalData(const UtilityEvaluator& evaluator)
	{
		aUtilities.emplace_back(evaluator.getUtility());
		const vector<double>& multipliers = evaluator.getMultipliers();
		if (aBestMultipliers.size() < multipliers.size())
			aBestMultipliers.resize(multipliers.size(), vector<double>());
		for (size_t i = 0; i < multipliers.size(); ++i)
			aBestMultipliers[i].emplace_back(multipliers[i]);
	}

	inline void addStatisticalData(const vector<UtilityEvaluator>& allEvaluators)
	{
		aAllMultipliers.emplace_back(vector<vector<double>>());
		for (auto pisja : allEvaluators)
			aAllMultipliers.back().emplace_back(pisja.getMultipliers());
	}

	void createCharts()
	{
		ofstream out;
		out.open(aName.c_str());
		// Utilities
		out << "ListPlot[{";
		for (size_t i = 0; i < aUtilities.size(); ++i)
		{
			for (auto u : aUtilities[i])
			{
				out << "{ " << i << ", " << u << " }";
				if (i != aUtilities.size() - 1)
					out << ", ";
			}
		}
		out << "}]" << endl << endl;

		// BestMultipliers
		// TODO: each multiplier with its own color
		out << "ListPlot[{";
		for (size_t i = 0; i < aBestMultipliers.size(); ++i)
		{
			for (auto u : aBestMultipliers[i])
			{
				out << "{ " << i << ", " << u << " }";
				if (i != aBestMultipliers.size() - 1)
					out << ", ";
			}
		}
		out << "}]" << endl << endl;

		// All multipliers
		out << "ListPlot[{";
		out << "}]" << endl << endl;



		out.close();
	}

private:
	vector<vector<size_t>> aUtilities;
	vector<vector<double>> aBestMultipliers;
	vector<vector<vector<double>>> aAllMultipliers;

	string aName;
};

