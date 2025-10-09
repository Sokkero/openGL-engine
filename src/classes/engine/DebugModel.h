#pragma once

#include "../SingletonManager.h"
#include "../helper/enums/EngineDrawEventsEnum.h"
#include "../helper/enums/LifecycleEventsEnum.h"

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace Engine
{
    class DebugModel : public SingletonBase
    {
        public:
            DebugModel() = default;
            ~DebugModel() = default;

            void setCalculationTimeData(DebugUtils::LifecycleEventsEnum lifecycleEventsEnum, double time)
            {
                m_calculationTimes[lifecycleEventsEnum] = time;
            }

            std::map<DebugUtils::LifecycleEventsEnum, double> getCalculationTimeData() const
            {
                return m_calculationTimes;
            }

            void setDrawSectionTimeData(const char* section, double time)
            {
                m_drawSectionTimes[section] = time;
            }

            std::map<const char*, double> getDrawSectionTimeData() const
            {
                return m_drawSectionTimes;
            }

        private:
            std::map<DebugUtils::LifecycleEventsEnum, double> m_calculationTimes;
            std::map<const char*, double> m_drawSectionTimes;
    };
} // namespace Engine
