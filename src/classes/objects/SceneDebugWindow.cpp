#include "SceneDebugWindow.h"

#include "../engine/EngineManager.h"
#include "../engine/WindowManager.h"
#include "../engine/rendering/RenderManager.h"
#include "../uiElements/UiElementButton.h"
#include "../uiElements/UiElementColorEdit.h"
#include "../uiElements/UiElementPlot.h"
#include "../uiElements/UiElementRadio.h"
#include "../uiElements/UiElementText.h"

using namespace Engine::Ui;

SceneDebugWindow::SceneDebugWindow()
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    m_lastTimeStamp = glfwGetTime();
    m_engineManager = getEngineManager();
    m_windowManager = getWindowManager();

    setWindowTitle("Scene Debugger");
    m_fpsCounter = std::make_shared<UiElementPlot>("FPS: inf");
    addContent(m_fpsCounter);

    m_frameTimer = std::make_shared<UiElementText>("ms/frame: inf");
    addContent(m_frameTimer);

    const auto& vsyncCallback = ([this](bool myBool) { m_windowManager->setVsync(myBool); });
    auto vsyncRadio = std::make_shared<UiElementRadio>(m_windowManager->getVsync(), "V-sync", vsyncCallback);
    addContent(vsyncRadio);

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

void SceneDebugWindow::update() { updateFrameCounter(); }

void SceneDebugWindow::updateFrameCounter()
{
    if(glfwGetTime() - m_lastTimeStamp >= 0.5)
    {
        int frames = m_engineManager->getFpsCount();
        std::string fpsText = "FPS: " + std::to_string(frames);
        m_fpsCounter->setText(fpsText);
        m_fpsCounter->addValue((float)frames);

        float msTime = 1000.f / (float)frames;
        fpsText = "Average ms/frame: " + std::to_string(msTime);
        m_frameTimer->setText(fpsText);

        m_lastTimeStamp = glfwGetTime();
    }
}