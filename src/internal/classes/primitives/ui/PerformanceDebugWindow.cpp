#include "PerformanceDebugWindow.h"

#include "classes/engine/EngineManager.h"
#include "classes/engine/PerformanceModel.h"
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

    m_lastTimeStamp = TimeUtils::GetSystemsTimestamp();
    m_engineManager = SingletonManager::get<EngineManager>();
    m_windowManager = SingletonManager::get<WindowManager>();
    m_debugModel = SingletonManager::get<PerformanceModel>();

    setWindowTitle("Performance Monitor");

    auto vsyncRadio = std::make_shared<UiElementRadio>(
            m_windowManager->getVsync(),
            "V-sync",
            std::bind(&PerformanceDebugWindow::onVsyncToggle, this, std::placeholders::_1)
    );
    addContent(vsyncRadio);

    m_fpsCounter = std::make_shared<UiElementPlot>("FPS: -1");
    addContent(m_fpsCounter);

    m_frameTimer = std::make_shared<UiElementText>("Average ms/frame: -1");
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

    m_timeDistributionGraph = std::make_shared<UiElementPieChart<int64_t>>("Frame time distribution");
    m_detailsSection->addContent(m_timeDistributionGraph);

    m_renderTimeDistributionGraph = std::make_shared<UiElementPieChart<int64_t>>("Render time distribution");
    m_detailsSection->addContent(m_renderTimeDistributionGraph);
}

void PerformanceDebugWindow::update()
{
    if(TimeUtils::GetDurationSince(m_lastTimeStamp, TimeUtils::Unit::ms) < 100)
    {
        return;
    }

    updateFrameCounter();

    if(m_detailsSection->getIsCurrentlyOpen())
    {
        updateTimeDistributionGraph();
    }
}

void PerformanceDebugWindow::onVsyncToggle(bool value)
{
    m_windowManager->setVsync(value);
}

void PerformanceDebugWindow::updateFrameCounter()
{
    int frames = m_engineManager->getFpsCount();
    std::string fpsText = "FPS: " + std::to_string(frames);
    m_fpsCounter->setText(fpsText);
    m_fpsCounter->addValue((float)frames);

    float msTime = 1000.f / (float)frames;
    fpsText = "Average ms/frame: " + std::to_string(msTime);
    m_frameTimer->setText(fpsText);

    m_lastTimeStamp = TimeUtils::GetSystemsTimestamp();
}

void PerformanceDebugWindow::updateTimeDistributionGraph()
{
    for(const auto& pair : m_debugModel->getEngineEventsTimeData())
    {
        m_timeDistributionGraph->addValue(DebugUtils::LifecycleEventsEnumToString(pair.first), pair.second);
    }

    for(const auto& pair : m_debugModel->getDrawSectionTimeData())
    {
        m_renderTimeDistributionGraph->addValue(pair.first, pair.second);
    }
}
