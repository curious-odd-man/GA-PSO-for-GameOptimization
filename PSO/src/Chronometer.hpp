#pragma once

#include <chrono>

namespace Chronometer
{
    typedef chrono::high_resolution_clock::time_point TimePoint;

    TimePoint now();
    double duration_seconds(const TimePoint& t1, const TimePoint& t2);
};

