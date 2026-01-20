#pragma once

#include <iostream>

namespace Engine::TimeUtils
{
    /**
     * @brief Returns a string representing the time duration
     * @param time The time value to be printed
     * @param includeNanoSeconds Wether or not to also show nanoseconds (might be inaccurate due to a lack of
     * precision)
     * @return A string, representing the time duration passed in (e.g. 9h 9m 9s 9ms 9ns)
     */
    static std::string GetHumanReadableTimeDuration(double time, const bool includeNanoSeconds = false)
    {
        const std::string hours = std::to_string(static_cast<int>(time) / 3600) + "h ";
        const std::string minutes = std::to_string(static_cast<int>(time) % 3600 / 60) + "m ";
        const std::string seconds = std::to_string(static_cast<int>(time) % 60) + "s ";
        const std::string milliseconds =
                std::to_string(static_cast<int>((time - static_cast<int>(time)) * 1000)) + "ms ";

        std::string timeString = hours + minutes + seconds + milliseconds;
        if(includeNanoSeconds)
        {
            const std::string nanoseconds =
                    std::to_string(static_cast<int>((time - static_cast<int>(time)) * 1e9) % 1000);
            timeString += nanoseconds + "ns";
        }

        return timeString;
    }
} // namespace Engine::TimeUtils
