#include "Common.hpp"
#include "Chronometer.hpp"

namespace Chronometer
{
    TimePoint now()
    {
        return chrono::high_resolution_clock::now();
    }

    double duration_milliseconds(const TimePoint& t1, const TimePoint& t2)
    {
#if 0
        double miliseconds = 321;
        double seconds = 1;
        double minutes = 45;
        double hours = 21;
        double days = 56;
        double total = miliseconds + 1000 * (seconds + 60 * (minutes + 60 * (hours + 24 * days)));
        return chrono::duration<double, milli>(total).count();
#else
        return chrono::duration<double, milli>(t2 - t1).count();
#endif
    }

    string duration(const TimePoint& t1, const TimePoint& t2)
    {
        const vector<pair<string, int>> units = {
            pair<string, int>("ms", 1),
            pair<string, int>("s", 1000),
            pair<string, int>("m", 60),
            pair<string, int>("h", 60),
            pair<string, int>("d", 24)
        };

        size_t majorUnits = (size_t)duration_milliseconds(t1, t2);
        int index = 0;
        vector<string> times(1, to_string(majorUnits) + units[index].first);

        while (++index < units.size())
        {
            size_t minorUnits = majorUnits % units[index].second;
            majorUnits /= units[index].second;
            times.back().assign(to_string(minorUnits) + units[index - 1].first);
            if (majorUnits > 0)
                times.emplace_back(to_string(majorUnits) + units[index].first);
            else
                break;
        }

        string result;
        for (auto itr = times.rbegin(); itr != times.rend(); ++itr)
            result.append(*itr + " ");

        return result;
    }
}
