#pragma once

#include "../nodeComponents/UiDebugWindow.h"

namespace Engine::Ui
{
    class UiElementPlot;
    class UiElementText;

    class SceneSettingsDebugWindow : public UiDebugWindow
    {
        public:
            SceneSettingsDebugWindow();
            ~SceneSettingsDebugWindow() = default;

            void update() override;

        private:
            std::shared_ptr<EngineManager> m_engineManager;
    };
} // namespace Engine::Ui