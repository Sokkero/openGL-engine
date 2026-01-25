#pragma once

#include <iomanip>
#include <utility>

namespace StringFormat
{
    inline bool isDataType(char c)
    {
        static const char LEN = 18;
        static const char PRINTF_FORMAT[LEN] { 's', 'd', 'i', 'f' };

        for(int i = 0; i < LEN; ++i)
        {
            if(c == PRINTF_FORMAT[i])
            {
                return true;
            }
        }
        return false;
    }

    inline std::string& replace(std::string& input, const std::string& value)
    {
        size_t pos = input.find("%");
        while(pos != std::string::npos && (pos + 1) < input.size())
        {
            if(isDataType(input[pos + 1]))
            {
                input = input.replace(pos, 2, value);
                break;
            }
            pos = input.find("%", pos + 1);
        }
        return input;
    }

    template<typename T>
    inline std::string toString(const T& val)
    {
        if(std::is_floating_point<T>::value)
        {
            char buffer[100] = { 0 };
            std::snprintf(buffer, 100, "%.1lf", (double)val);
            return buffer;
        }
        return std::to_string(val);
    }

    inline std::string toString(const char* val)
    {
        // null is not allowed in std::string
        if(!val)
        {
            return "";
        }
        return std::string(val);
    }

    inline std::string toString(char val) { return std::string(1, val); }

    inline const std::string& toString(const std::string& val) { return val; }

    template<class T>
    inline std::string toString(const T* val)
    {
        // 19 because on 64 bits we have 8 bytes ^= 16 chars for hex representation of the address + "0x" = 2
        // chars and final \0 termination 1 char
        char buffer[19] { 0 };
        snprintf(buffer, 19, "0x%x", val);
        return buffer;
    }

    template<class T>
    inline std::string toString(T* val)
    {
        // 19 because on 64 bits we have 8 bytes ^= 16 chars for hex representation of the address + "0x" = 2
        // chars and final \0 termination 1 char
        char buffer[19] { 0 };
        snprintf(buffer, 19, "0x%llx", (unsigned long long)val);
        return buffer;
    }

    inline std::string formatString(const std::string& finalString, const size_t) { return finalString; }

    template<typename T, typename... Args>
    inline std::string formatString(std::string& format, const size_t idx, T&& val, Args&&... args)
    {
        std::string value = toString(val);
        return formatString(replace(format, value), idx + 1, std::forward<Args>(args)...);
    }
} // namespace StringFormat

template<typename... Args>
static std::string stringf(const std::string& format, Args&&... args)
{
    std::string result(format);
    StringFormat::formatString(result, 0, std::forward<Args>(args)...);
    return result;
}
