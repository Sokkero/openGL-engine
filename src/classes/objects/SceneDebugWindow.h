#pragma once

#include "../nodeComponents/UiDebugWindow.h"

namespace Engine::Ui
{
    class UiElementPlot;
    class UiElementText;

    class SceneDebugWindow : public UiDebugWindow
    {
        public:
            SceneDebugWindow();
            ~SceneDebugWindow() = default;

            void update() override;

        private:
            std::shared_ptr<EngineManager> m_engineManager;
            std::shared_ptr<WindowManager> m_windowManager;
            std::shared_ptr<UiElementPlot> m_fpsCounter;
            std::shared_ptr<UiElementText> m_frameTimer;

            // Fps counter stuff
            void updateFrameCounter();
            double m_lastTimeStamp;
    };
} // namespace Engine::Ui