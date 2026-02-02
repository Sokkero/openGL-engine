#pragma once

#include "classes/nodeComponents/UiDebugWindow.h"

namespace Engine
{
    class RenderManager;

    namespace Ui
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
                void onWireframeToggle(bool value) const;
                void onDebugUiToggle(bool value) const;

                std::shared_ptr<RenderManager> m_renderManager;
        };
    } // namespace Ui
} // namespace Engine