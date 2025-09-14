#pragma once

#include "../SingletonManager.h"

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace Engine
{
    class DebugModel : public SingletonBase
    {
        public:
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

            static char* EnumToString(LifecycleEventsEnum lifecycleEventsEnum)
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

            static std::vector<const char*> getAllEnumStrings()
            {
                std::vector<const char*> names;
                names.emplace_back(EnumToString(ImGuiNewFrame));
                names.emplace_back(EnumToString(UserEventsUpdate));
                names.emplace_back(EnumToString(EngineUpdate));
                names.emplace_back(EnumToString(EngineDraw));
                names.emplace_back(EnumToString(ImGuiDraw));
                names.emplace_back(EnumToString(BufferSwap));
                names.emplace_back(EnumToString(EngineLateUpdate));
                names.emplace_back(EnumToString(glfwPoll));

                return names;
            }

            DebugModel() = default;
            ~DebugModel() = default;

            void setCalculationTimeData(LifecycleEventsEnum lifecycleEventsEnum, double time)
            {
                m_calculationTimes[lifecycleEventsEnum] = time;
            }

            std::map<LifecycleEventsEnum, double> getCalculationTimeData() const
            {
                return m_calculationTimes;
            }

        private:
            std::map<LifecycleEventsEnum, double> m_calculationTimes;
    };
} // namespace Engine
