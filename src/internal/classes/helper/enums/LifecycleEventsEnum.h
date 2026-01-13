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
} // namespace DebugUtils
