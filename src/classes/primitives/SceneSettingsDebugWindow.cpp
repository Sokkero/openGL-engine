#include "SceneSettingsDebugWindow.h"

#include "../engine/EngineManager.h"
#include "../engine/WindowManager.h"
#include "../engine/rendering/RenderManager.h"
#include "../uiElements/UiElementColorEdit.h"
#include "../uiElements/UiElementRadio.h"

using namespace Engine::Ui;

SceneSettingsDebugWindow::SceneSettingsDebugWindow()
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    setIsWindowClosable(false);

    m_engineManager = getEngineManager();

    setWindowTitle("Scene Settings");

    auto gridRadio = std::make_shared<UiElementRadio>(
            m_engineManager->isGridVisible(),
            "Grid",
            std::bind(&SceneSettingsDebugWindow::onGridToggle, this, std::placeholders::_1)
    );
    addContent(gridRadio);

    auto wireframeRadio = std::make_shared<UiElementRadio>(
            m_engineManager->getRenderManager()->getWireframeMode(),
            "Wireframe",
            std::bind(&SceneSettingsDebugWindow::onWireframeToggle, this, std::placeholders::_1)
    );
    addContent(wireframeRadio);

    float* currClearColor = m_engineManager->getClearColor();
    const auto& clearColorCallback = ([this](float value[4]) { m_engineManager->setClearColor(value); });
    std::shared_ptr<UiElementColorEdit> clearColorEdit =
            std::make_shared<UiElementColorEdit>(currClearColor, "Clear Color", clearColorCallback);
    addContent(clearColorEdit);
}

void SceneSettingsDebugWindow::onWireframeToggle(bool value) const
{
    m_engineManager->getRenderManager()->setWireframeMode(value);
}

void SceneSettingsDebugWindow::onGridToggle(bool value) const { m_engineManager->setGridVisibility(value); }

void SceneSettingsDebugWindow::update() {}