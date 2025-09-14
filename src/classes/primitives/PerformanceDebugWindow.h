#pragma once

#include "../nodeComponents/UiDebugWindow.h"

namespace Engine
{
    class EngineManager;
    class WindowManager;
    class DebugModel;

    namespace Ui
    {
        class UiElementPlot;
        class UiElementPieChart;
        class UiElementText;

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
                std::shared_ptr<DebugModel> m_debugModel;
                std::shared_ptr<UiElementPlot> m_fpsCounter;
                std::shared_ptr<UiElementText> m_frameTimer;
                std::shared_ptr<UiElementPieChart> m_timeDistributionGraph;

                double m_lastTimeStamp;
        };
    } // namespace Ui
} // namespace Engine