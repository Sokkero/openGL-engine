#pragma once

#include "classes/SingletonManager.h"
#include "utils/enums/LifecycleEventsEnum.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <map>

namespace Engine
{
    class PerformanceModel : public SingletonBase
    {
        public:
            PerformanceModel() = default;
            ~PerformanceModel() = default;

            void setEngineEventTimeData(DebugUtils::LifecycleEventsEnum lifecycleEventsEnum, int64_t ms)
            {
                m_engineEventTimes[lifecycleEventsEnum] = ms;
            }

            std::map<DebugUtils::LifecycleEventsEnum, int64_t> getEngineEventsTimeData() const
            {
                return m_engineEventTimes;
            }

            void setDrawSectionTimeData(const char* section, int64_t time)
            {
                m_drawSectionTimes[section] = time;
            }

            std::map<const char*, int64_t> getDrawSectionTimeData() const { return m_drawSectionTimes; }

        private:
            std::map<DebugUtils::LifecycleEventsEnum, int64_t> m_engineEventTimes;
            std::map<const char*, int64_t> m_drawSectionTimes;
    };
} // namespace Engine
