#include "Common.hpp"
#include "Chronometer.hpp"

namespace Chronometer
{
    TimePoint now()
    {
        return chrono::high_resolution_clock::now();
    }

    double duration_seconds(const TimePoint& t1, const TimePoint& t2)
    {
        return chrono::duration<double>(t2 - t1).count();
    }
}
