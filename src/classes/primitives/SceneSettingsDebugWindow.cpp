#include "SceneSettingsDebugWindow.h"

#include "../engine/EngineManager.h"
#include "../engine/WindowManager.h"
#include "../engine/rendering/RenderManager.h"
#include "../uiElements/UiElementRadio.h"
#include "../uiElements/UiElementColorEdit.h"

using namespace Engine::Ui;

SceneSettingsDebugWindow::SceneSettingsDebugWindow()
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    setIsWindowClosable(false);

    m_engineManager = getEngineManager();

    setWindowTitle("Scene Settings");

    const auto& wireframeCallback = ([this](bool myBool)
                                     { m_engineManager->getRenderManager()->setWireframeMode(myBool); });
    auto wireframeRadio = std::make_shared<UiElementRadio>(
            m_engineManager->getRenderManager()->getWireframeMode(),
            "Wireframe",
            wireframeCallback
    );
    addContent(wireframeRadio);

    float* currClearColor = m_engineManager->getClearColor();
    const auto& clearColorCallback = ([this](float value[4]) { m_engineManager->setClearColor(value); });
    std::shared_ptr<UiElementColorEdit> clearColorEdit =
            std::make_shared<UiElementColorEdit>(currClearColor, "Clear Color", clearColorCallback);
    addContent(clearColorEdit);
}

void SceneSettingsDebugWindow::update() { }