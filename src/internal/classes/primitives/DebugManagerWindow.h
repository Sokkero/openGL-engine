#pragma once

#include "classes/nodeComponents/UiDebugWindow.h"

namespace Engine::Ui
{
    class PerformanceDebugWindow;
    class SceneSettingsDebugWindow;
    class UiElementRadio;

    class DebugManagerWindow : public UiDebugWindow
    {
        public:
            DebugManagerWindow();
            ~DebugManagerWindow() = default;

            void update() override;

        private:
            void onPerformanceWindowRadioClick(bool newValue);
            std::shared_ptr<PerformanceDebugWindow> m_performanceWindow;
            std::shared_ptr<UiElementRadio> m_performanceWindowRadio;

            void onSceneSettingsWindowRadioClick(bool newValue);
            std::shared_ptr<SceneSettingsDebugWindow> m_sceneSettingsWindow;
            std::shared_ptr<UiElementRadio> m_sceneSettingsWindowRadio;
    };
} // namespace Engine::Ui