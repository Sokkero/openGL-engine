#pragma once

#include <vector>

namespace DebugUtils
{
    enum EngineDrawEventsEnum
    {
        DepthSorting,
        DrawOpaque,
        DrawTranslucent,
        DrawGrid,
        DrawUI
    };

    static char* EngineDrawEventsEnumToString(EngineDrawEventsEnum engineDrawEventsEnum)
    {
        switch(engineDrawEventsEnum)
        {
            case DepthSorting:
                return (char*)"DepthSorting";
            case DrawOpaque:
                return (char*)"DrawOpaque";
            case DrawTranslucent:
                return (char*)"DrawTranslucent";
            case DrawGrid:
                return (char*)"DrawGrid";
            case DrawUI:
                return (char*)"DrawUI";
        }
    }

    static std::vector<const char*> getAllEngineDrawEventsEnumStrings()
    {
        std::vector<const char*> names;
        names.emplace_back(EngineDrawEventsEnumToString(DepthSorting));
        names.emplace_back(EngineDrawEventsEnumToString(DrawOpaque));
        names.emplace_back(EngineDrawEventsEnumToString(DrawTranslucent));
        names.emplace_back(EngineDrawEventsEnumToString(DrawGrid));
        names.emplace_back(EngineDrawEventsEnumToString(DrawUI));

        return names;
    }
} // namespace DebugUtils
