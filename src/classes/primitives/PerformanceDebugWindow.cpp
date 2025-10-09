#include "PerformanceDebugWindow.h"

#include "../engine/DebugModel.h"
#include "../engine/EngineManager.h"
#include "../engine/WindowManager.h"
#include "../engine/rendering/RenderManager.h"
#include "../uiElements/UiElementButton.h"
#include "../uiElements/UiElementCollapsableSection.h"
#include "../uiElements/UiElementPieChart.h"
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

    m_timeDistributionGraph = std::make_shared<UiElementPieChart>(
            "Frame time distribution (ms)",
            DebugUtils::getAllLifecycleEventsEnumStrings()
    );
    m_detailsSection->addContent(m_timeDistributionGraph);

    const std::vector<const char*> names = {"0.5", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    m_testGraph = std::make_shared<UiElementPieChart>(
            "Shader sections",
            names
    );
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
