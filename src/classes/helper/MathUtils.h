
#include <iostream>
#include <numeric>

namespace Engine::MathUtils
{
    template<typename T>
    static T GetAverage(const std::vector<T>& numbers)
    {
        static_assert(std::is_arithmetic<T>::value, "Template type must be a numeric type.");

        if(numbers.empty())
        {
            return 0.0; // Return 0 if the vector is empty
        }

        T sum = std::accumulate(numbers.begin(), numbers.end(), T(0));
        return static_cast<double>(sum) / numbers.size();
    }

    template<typename T>
    static T GetSum(const std::vector<T>& numbers)
    {
        static_assert(std::is_arithmetic<T>::value, "Template type must be a numeric type.");

        if(numbers.empty())
        {
            return 0.0; // Return 0 if the vector is empty
        }

        return static_cast<double>(std::accumulate(numbers.begin(), numbers.end(), T(0)));
    }

    template<typename T>
    static T GetMax(const std::vector<T>& numbers)
    {
        static_assert(std::is_arithmetic<T>::value, "Template type must be a numeric type.");

        T max = 0;
        for(const auto& number : numbers)
        {
            if(number > max)
            {
                max = number;
            }
        }
        return static_cast<double>(max);
    }

    template<typename T>
    static T GetMin(const std::vector<T>& numbers)
    {
        static_assert(std::is_arithmetic<T>::value, "Template type must be a numeric type.");

        T min = std::numeric_limits<T>::max();
        for(const auto& number : numbers)
        {
            if(number < min)
            {
                min = number;
            }
        }
        return static_cast<double>(min);
    }
} // namespace Engine::MathUtils
