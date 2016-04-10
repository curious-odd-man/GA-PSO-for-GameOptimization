
#pragma once
#include "Common.hpp"







template <typename GeneType>
class Gene
{
public:
	Gene(GeneType value, GeneType minValue, GeneType maxValue) : aValue(value)
	{
		if (aMinMaxDefined)
			return;
		aMinValue = minValue;
		aMaxValue = maxValue;
		aMinMaxDefined = true;
	};

	Gene() : Gene(getRandom(), GeneType(0), GeneType(0)) {};

	GeneType getValue() { return aValue; }

	Gene<GeneType>& operator++();
	Gene<GeneType> operator++(int);

	Gene<GeneType>& operator=(const Gene<GeneType>& other);

	static GeneType getRandom();

	template <class GeneType2>
	friend ostream& operator<<(ostream& os, const Gene<GeneType2>& object);

protected:
private:
	GeneType aValue;
	static GeneType aMinValue;
	static GeneType aMaxValue;
	static bool aMinMaxDefined;
};



template <typename GeneType>
bool Gene<GeneType>::aMinMaxDefined = true;

template <typename GeneType>
GeneType Gene<GeneType>::aMinValue = -1.0;

template <typename GeneType>
GeneType Gene<GeneType>::aMaxValue = +1.0;



template <typename GeneType>
Gene<GeneType>& Gene<GeneType>::operator++()
{
	aValue += getRandom() / 3; // FIXME: No magic numbers

	aValue = aValue < aMinValue ? aMinValue : aValue;
	aValue = aValue > aMaxValue ? aMaxValue : aValue;

	return *this;
}

template <typename GeneType>
Gene<GeneType> Gene<GeneType>::operator++(int)
{
	Gene<GeneType> tmp(*this);
	operator++();
	return tmp;
}

template<typename GeneType>
Gene<GeneType>& Gene<GeneType>::operator=(const Gene<GeneType>& other)
{
	aValue = other.aValue;
	return *this;
}

template <typename GeneType>
GeneType Gene<GeneType>::getRandom()
{
	static uniform_real_distribution<GeneType> unif(aMinValue, aMaxValue); // TODO: maybe they should be static too ?
	static default_random_engine re(time(0));
	return unif(re);
}



template <class GeneType>
ostream& operator<<(ostream& os, const Gene<GeneType>& object)
{
	return os << "Value: " << object.aValue << " [" << object.aMinValue << ", " << object.aMaxValue << "]";
}



