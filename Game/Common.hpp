#pragma once

using namespace std;

//#define TEST_PARAMS
//#define TEST

#ifndef _DEBUG
#define USE_PARALEL_OPTIMIZATION
#endif

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
#include <climits>
#include <thread>
#include <future>

#include "UtilityEvaluator.hpp"
#include "Field.hpp"
#include "Figure.hpp"
#include "Game.hpp"
#include "DemonstrationGame.hpp"
#include "OptimizationGame.hpp"

const string DELIMITER = "================================================================================\n";

template<typename T, typename TT, typename TTT>
bool in_range(T const& s, TT const& e, TTT const& v)
{
    return s <= v && v <= e;
}

bool key_pressed(int* code);

void testSolution(UtilityEvaluator& testObject, size_t count, size_t width, size_t height, size_t figureSize,
                  unsigned char colorsCount, size_t gameCount);
