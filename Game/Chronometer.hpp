#pragma once

#include <chrono>

namespace Chronometer
{
    typedef chrono::high_resolution_clock::time_point TimePoint;

    TimePoint now();
    double duration_milliseconds(const TimePoint& t1, const TimePoint& t2);
    string duration(const TimePoint& t1, const TimePoint& t2);
}
