#include "PerformanceDebugWindow.h"

#include "classes/engine/DebugModel.h"
#include "classes/engine/EngineManager.h"
#include "classes/engine/WindowManager.h"
#include "classes/engine/rendering/RenderManager.h"
#include "classes/uiElements/UiElementButton.h"
#include "classes/uiElements/UiElementCollapsableSection.h"
#include "classes/uiElements/UiElementPieChart.h"
#include "classes/uiElements/UiElementPlot.h"
#include "classes/uiElements/UiElementRadio.h"
#include "classes/uiElements/UiElementText.h"

using namespace Engine::Ui;

PerformanceDebugWindow::PerformanceDebugWindow()
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    setIsWindowClosable(false);

    m_lastTimeStamp = glfwGetTime();
    m_engineManager = SingletonManager::get<EngineManager>();
    m_windowManager = SingletonManager::get<WindowManager>();
    m_debugModel = SingletonManager::get<DebugModel>();

    setWindowTitle("Performance Monitor");

    auto vsyncRadio = std::make_shared<UiElementRadio>(
            m_windowManager->getVsync(),
            "V-sync",
            std::bind(&PerformanceDebugWindow::onVsyncToggle, this, std::placeholders::_1)
    );
    addContent(vsyncRadio);

    m_fpsCounter = std::make_shared<UiElementPlot>("FPS: inf");
    addContent(m_fpsCounter);

    m_frameTimer = std::make_shared<UiElementText>("ms/frame: inf");
    addContent(m_frameTimer);

    m_detailsSection = std::make_shared<UiElementCollapsableSection>("Details");
    m_detailsSection->setCallback(
            [this](bool isOpen)
            {
                if(!isOpen)
                {
                    m_timeDistributionGraph->clearValues();
                }
            }
    );
    addContent(m_detailsSection);

    m_timeDistributionGraph = std::make_shared<UiElementPieChart>("Frame time distribution (ms)");
    m_detailsSection->addContent(m_timeDistributionGraph);

    m_testGraph = std::make_shared<UiElementPieChart>("Shader sections");
    m_detailsSection->addContent(m_testGraph);
}

void PerformanceDebugWindow::update()
{
    if(glfwGetTime() - m_lastTimeStamp < 0.1)
    {
        return;
    }

    updateFrameCounter();

    if(m_detailsSection->getIsCurrentlyOpen())
    {
        updateTimeDistributionGraph();
    }
}

void PerformanceDebugWindow::onVsyncToggle(bool value) { m_windowManager->setVsync(value); }

void PerformanceDebugWindow::updateFrameCounter()
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

void PerformanceDebugWindow::updateTimeDistributionGraph()
{
    for(const auto& pair : m_debugModel->getCalculationTimeData())
    {
        // *1000 ms -> s
        m_timeDistributionGraph->addValue(DebugUtils::LifecycleEventsEnumToString(pair.first), pair.second * 1000);
    }

    for(const auto& pair : m_debugModel->getDrawSectionTimeData())
    {
        // *1000 ms -> s
        m_testGraph->addValue(pair.first, pair.second);
    }
}
