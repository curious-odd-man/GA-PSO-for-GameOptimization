#pragma once

using namespace std;

#include <vector>
#include <cstdlib>
#include <iostream>
#include <random>
#include <ctime>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <csignal>

#include "UtilityEvaluator.hpp"
#include "Field.hpp"
#include "Figure.hpp"
#include "Game.hpp"
#include "DemonstrationGame.hpp"
#include "OptimizationGame.hpp"

template<typename T, typename TT, typename TTT>
bool in_range(T const& s, TT const& e, TTT const& v)
{
    return s <= v && v <= e;
}

bool key_pressed(int* code);
