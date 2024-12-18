#include "PerformanceDebugWindow.h"

#include "../engine/EngineManager.h"
#include "../engine/WindowManager.h"
#include "../engine/rendering/RenderManager.h"
#include "../uiElements/UiElementButton.h"
#include "../uiElements/UiElementPlot.h"
#include "../uiElements/UiElementRadio.h"
#include "../uiElements/UiElementText.h"

using namespace Engine::Ui;

PerformanceDebugWindow::PerformanceDebugWindow()
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    setIsWindowClosable(false);

    m_lastTimeStamp = glfwGetTime();
    m_engineManager = SingletonManager::get<EngineManager>();
    m_windowManager = SingletonManager::get<WindowManager>();

    setWindowTitle("Performance Monitor");

    m_fpsCounter = std::make_shared<UiElementPlot>("FPS: inf");
    addContent(m_fpsCounter);

    m_frameTimer = std::make_shared<UiElementText>("ms/frame: inf");
    addContent(m_frameTimer);

    auto vsyncRadio = std::make_shared<UiElementRadio>(
            m_windowManager->getVsync(),
            "V-sync",
            std::bind(&PerformanceDebugWindow::onVsyncToggle, this, std::placeholders::_1)
    );
    addContent(vsyncRadio);
}

void PerformanceDebugWindow::update() { updateFrameCounter(); }

void PerformanceDebugWindow::onVsyncToggle(bool value) { m_windowManager->setVsync(value); }

void PerformanceDebugWindow::updateFrameCounter()
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