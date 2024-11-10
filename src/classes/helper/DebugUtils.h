
#include <iostream>

class DebugUtils
{
    public:
        /**
             * @brief Prints a double time value in human readable format to the console
             * @param time The time value to be printed
             * @param text The text to be displayed with the output
             * @param includeNanoSeconds Wether or not to also show nanoseconds (might be inaccurate due to a lack of precision)
             * @return An unsigned int, the unique ID of the node
         */
        static void PrintHumanReadableTimeDuration(double time, const std::string& text = "", const bool includeNanoSeconds = false)
        {
            const int hours = static_cast<int>(time) / 3600;
            const int minutes = (static_cast<int>(time) % 3600) / 60;
            const int seconds = static_cast<int>(time) % 60;
            const int milliseconds = static_cast<int>((time - static_cast<int>(time)) * 1000);
            const int nanoseconds = static_cast<int>((time - static_cast<int>(time)) * 1e9) % 1000;

            if(includeNanoSeconds) {
                std::cout << text
                          << hours << "h "
                          << minutes << "m "
                          << seconds << "s "
                          << milliseconds << "ms "
                          << nanoseconds << "ns" << std::endl;
            }
            else
            {
                std::cout << text
                          << hours << "h "
                          << minutes << "m "
                          << seconds << "s "
                          << milliseconds << "ms " << std::endl;
            }
        }
};
