#include "DebugManagerWindow.h"

#include "../engine/EngineManager.h"
#include "../engine/WindowManager.h"
#include "../engine/rendering/RenderManager.h"
#include "../uiElements/UiElementPlot.h"
#include "../uiElements/UiElementRadio.h"
#include "../uiElements/UiElementText.h"
#include "PerformanceDebugWindow.h"
#include "SceneSettingsDebugWindow.h"

using namespace Engine::Ui;

DebugManagerWindow::DebugManagerWindow()
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    setWindowTitle("Debug Manager");

    m_performanceWindowRadio = std::make_shared<UiElementRadio>(false, "Performance Monitor", std::bind(&DebugManagerWindow::onPerformanceWindowRadioClick, this, std::placeholders::_1));
    addContent(m_performanceWindowRadio);

    m_sceneSettingsWindowRadio = std::make_shared<UiElementRadio>(false, "Scene Settings", std::bind(&DebugManagerWindow::onSceneSettingsWindowRadioClick, this, std::placeholders::_1));
    addContent(m_sceneSettingsWindowRadio);
}

void DebugManagerWindow::update() {}

void DebugManagerWindow::onPerformanceWindowRadioClick(bool newValue)
{
    if(newValue)
    {
        m_performanceWindow = std::make_shared<PerformanceDebugWindow>();
        addChild(m_performanceWindow);
    }
    else
    {
        deleteChild(m_performanceWindow);
        m_performanceWindow = nullptr;
    }
}

void DebugManagerWindow::onSceneSettingsWindowRadioClick(bool newValue)
{
    if(newValue)
    {
        m_sceneSettingsWindow = std::make_shared<SceneSettingsDebugWindow>();
        addChild(m_sceneSettingsWindow);
    }
    else
    {
        deleteChild(m_sceneSettingsWindow);
        m_sceneSettingsWindow = nullptr;
    }
}
