#pragma once

#include <iostream>
#include <chrono>

namespace Engine::TimeUtils
{
    using namespace std::chrono;

    using SysTimestamp = time_point<steady_clock, duration<long long, std::ratio<1, 1000000000>>>;

    enum class TimeUnit {
        Nanoseconds,
        Microseconds,
        Milliseconds,
        Seconds
    };

    /**
     * @brief Prints the minimum measurable time in theory and practice
     */
    static void CheckClockAccuracy()
    {
        // Theoretical minimum
        auto resolution_ns = duration_cast<nanoseconds>(high_resolution_clock::duration(1)).count();
        LOG_DEBUG("TimeUtils", "Theoretical time accuracy: " + std::to_string(resolution_ns) + "ns");

        // Practical minimum
        long long min_difference = LLONG_MAX;
        for (int i = 0; i < 10000; i++)
        {
            auto t1 = high_resolution_clock::now();
            auto t2 = high_resolution_clock::now();

            auto diff_ns = duration_cast<nanoseconds>(t2 - t1).count();
            if (diff_ns > 0 && diff_ns < min_difference)
            {
                min_difference = diff_ns;
            }
        }

        LOG_DEBUG("TimeUtils", "Actual time accuracy: " + std::to_string(min_difference) + "ns");
    }

    /**
     * @brief Returns a string representing the time duration (e.g. 9h 9m 9s 9ms)
     */
    static std::string GetHumanReadableTimeDuration(double time)
    {
        const int hours = static_cast<int>(time) / 3600;
        const int minutes = static_cast<int>(time) / 3600;
        const int seconds = static_cast<int>(time) / 3600;
        const int milliseconds = static_cast<int>(time) / 3600;

        const std::string hoursString = std::to_string(hours) + "h ";
        const std::string minutesString = std::to_string(minutes) + "m ";
        const std::string secondsString = std::to_string(seconds) + "s ";
        const std::string millisecondsString = std::to_string(milliseconds) + "ms ";

        std::string timeString;
        if(hours > 0)
        {
            timeString += hoursString;
        }
        if(minutes > 0)
        {
            timeString += minutesString;
        }
        if(seconds > 0)
        {
            timeString += secondsString;
        }
        if(milliseconds > 0)
        {
            timeString += millisecondsString;
        }

        return timeString;
    }

    static SysTimestamp GetSystemsTimestamp()
    {
        return high_resolution_clock::now();
    }

    /**
     * @brief Returns the amount of time passed in the given unit
     */
    static int64_t GetDuration(SysTimestamp start, SysTimestamp end, TimeUnit timeUnit)
    {
        auto duration = end - start;
        switch(timeUnit)
        {
            case TimeUnit::Nanoseconds:
                return duration_cast<nanoseconds>(duration).count();
                break;
            case TimeUnit::Microseconds:
                return duration_cast<microseconds>(duration).count();
                break;
            case TimeUnit::Milliseconds:
                return duration_cast<milliseconds>(duration).count();
                break;
            case TimeUnit::Seconds:
                return duration_cast<seconds>(duration).count();
                break;
        }
    }

    /**
     * @brief Returns the amount of time passed in fractured seconds (e.g. 2.7168 seconds)
     */
    static double GetFracturedDuration(SysTimestamp start, SysTimestamp end)
    {
        auto duration = end - start;
        return std::chrono::duration<double>(duration).count();
    }
} // namespace Engine::TimeUtils
