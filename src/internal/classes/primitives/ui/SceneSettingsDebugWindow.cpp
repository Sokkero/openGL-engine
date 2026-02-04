#include "SceneSettingsDebugWindow.h"

#include "classes/engine/EngineManager.h"
#include "classes/engine/WindowManager.h"
#include "classes/engine/rendering/RenderManager.h"
#include "classes/uiElements/UiElementColorEdit.h"
#include "classes/uiElements/UiElementRadio.h"

using namespace Engine::Ui;

SceneSettingsDebugWindow::SceneSettingsDebugWindow()
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    setIsWindowClosable(false);

    m_renderManager = SingletonManager::get<RenderManager>();

    setWindowTitle("Scene Settings");

    auto debugUiRadio = std::make_shared<UiElementRadio>(
            m_renderManager->isDebugUiVisible(),
            "Debug UI",
            std::bind(&SceneSettingsDebugWindow::onDebugUiToggle, this, std::placeholders::_1)
    );
    addContent(debugUiRadio);

    auto wireframeRadio = std::make_shared<UiElementRadio>(
            m_renderManager->getWireframeMode(),
            "Wireframe",
            std::bind(&SceneSettingsDebugWindow::onWireframeToggle, this, std::placeholders::_1)
    );
    addContent(wireframeRadio);

    float* currClearColor = m_renderManager->getClearColor();
    const auto& clearColorCallback = ([this](float value[4]) { m_renderManager->setClearColor(value); });
    std::shared_ptr<UiElementColorEdit> clearColorEdit = std::make_shared<UiElementColorEdit>(
            currClearColor,
            "Clear Color",
            clearColorCallback
    );
    addContent(clearColorEdit);
}

void SceneSettingsDebugWindow::onWireframeToggle(bool value) const
{
    m_renderManager->setWireframeMode(value);
}

void SceneSettingsDebugWindow::onDebugUiToggle(bool value) const
{
    m_renderManager->setDebugUiVisibility(value);
}

void SceneSettingsDebugWindow::update() {}