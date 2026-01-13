#pragma once

#include <vector>

namespace DebugUtils
{
    enum LifecycleEventsEnum
    {
        ImGuiNewFrame,
        UserEventsUpdate,
        EngineUpdate,
        EngineDraw,
        ImGuiDraw,
        BufferSwap,
        EngineLateUpdate,
        glfwPoll
    };

    static char* LifecycleEventsEnumToString(LifecycleEventsEnum lifecycleEventsEnum)
    {
        switch(lifecycleEventsEnum)
        {
            case ImGuiNewFrame:
                return (char*)"ImGuiNewFrame";
            case UserEventsUpdate:
                return (char*)"UserEventsUpdate";
            case EngineUpdate:
                return (char*)"EngineUpdate";
            case EngineDraw:
                return (char*)"EngineDraw";
            case ImGuiDraw:
                return (char*)"ImGuiDraw";
            case BufferSwap:
                return (char*)"BufferSwap";
            case EngineLateUpdate:
                return (char*)"EngineLateUpdate";
            case glfwPoll:
                return (char*)"glfwPoll";
        }
    }

    static std::vector<const char*> getAllLifecycleEventsEnumStrings()
    {
        std::vector<const char*> names;
        names.emplace_back(LifecycleEventsEnumToString(ImGuiNewFrame));
        names.emplace_back(LifecycleEventsEnumToString(UserEventsUpdate));
        names.emplace_back(LifecycleEventsEnumToString(EngineUpdate));
        names.emplace_back(LifecycleEventsEnumToString(EngineDraw));
        names.emplace_back(LifecycleEventsEnumToString(ImGuiDraw));
        names.emplace_back(LifecycleEventsEnumToString(BufferSwap));
        names.emplace_back(LifecycleEventsEnumToString(EngineLateUpdate));
        names.emplace_back(LifecycleEventsEnumToString(glfwPoll));

        return names;
    }
} // namespace DebugUtils
