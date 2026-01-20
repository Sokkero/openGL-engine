#pragma once

#include "classes/utils/StringFormat.h"

#include <cassert>
#include <iostream>

#define ESC "\033["
#define ERROR_TEXT_COLOR "91m"
#define WARN_TEXT_COLOR "93m"
#define DEBUG_TEXT_COLOR "97m"
#define RESET "\033[m"

inline size_t MIN_TAG_LENGTH = 9;

namespace Logger
{
    enum LOG_TYPE
    {
        LOG_TYPE_DEBUG,
        LOG_TYPE_WARNING,
        LOG_TYPE_ERROR
    };

    static void LOG(LOG_TYPE logType, const std::string& tag, const std::string& msg)
    {
        std::string logText = ESC;
        switch(logType)
        {
            case LOG_TYPE_DEBUG:
                logText += DEBUG_TEXT_COLOR;
                break;
            case LOG_TYPE_WARNING:
                logText += WARN_TEXT_COLOR;
                break;
            case LOG_TYPE_ERROR:
                logText += ERROR_TEXT_COLOR;
                break;
        }

        if(!tag.empty())
        {
            std::string tagText = tag;
            if(tag.size() > MIN_TAG_LENGTH)
            {
                MIN_TAG_LENGTH = tag.size();
            }
            else
            {
                tagText.resize(MIN_TAG_LENGTH, ' ');
            }

            logText += tagText + " || ";
        }

        std::cout << logText << msg << RESET << std::endl;
    }

    static void ASSERT(bool condition, const std::string& msg)
    {
        if(!condition)
        {
            LOG(LOG_TYPE::LOG_TYPE_ERROR, "", msg);
            assert(false);
        }
    }
} // namespace Logger

#define LOG_DEBUG(tag, msg) Logger::LOG(Logger::LOG_TYPE::LOG_TYPE_DEBUG, tag, msg)
#define LOG_WARN(tag, msg) Logger::LOG(Logger::LOG_TYPE::LOG_TYPE_WARNING, tag, msg)
#define LOG_ERROR(tag, msg) Logger::LOG(Logger::LOG_TYPE::LOG_TYPE_ERROR, tag, msg)

#define ENGINE_ASSERT(condition, msg) Logger::ASSERT(condition, msg);
