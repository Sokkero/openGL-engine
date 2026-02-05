#pragma once

#include "classes/nodeComponents/UiDebugWindow.h"
#include "classes/uiElements/UiElementPieChart.h"
#include "utils/TimeUtils.h"

namespace Engine
{
    class EngineManager;
    class WindowManager;
    class PerformanceModel;

    namespace Ui
    {
        class UiElementPlot;
        class UiElementText;
        class UiElementCollapsableSection;

        class PerformanceDebugWindow : public UiDebugWindow
        {
            public:
                PerformanceDebugWindow();
                ~PerformanceDebugWindow() = default;

                void update() override;

            private:
                void onVsyncToggle(bool value);

                void updateFrameCounter();
                void updateTimeDistributionGraph();

                std::shared_ptr<EngineManager> m_engineManager;
                std::shared_ptr<WindowManager> m_windowManager;
                std::shared_ptr<PerformanceModel> m_debugModel;
                std::shared_ptr<UiElementPlot> m_fpsCounter;
                std::shared_ptr<UiElementText> m_frameTimer;
                std::shared_ptr<UiElementPieChart<int64_t>> m_timeDistributionGraph;
                std::shared_ptr<UiElementPieChart<int64_t>> m_renderTimeDistributionGraph;
                std::shared_ptr<UiElementCollapsableSection> m_detailsSection;

                TimeUtils::SysTimestamp m_lastTimeStamp;
        };
    } // namespace Ui
} // namespace Engine