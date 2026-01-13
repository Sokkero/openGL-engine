
#include "EngineManager.h"

#include "../../customCode/testScene/TestSceneOrigin.h"
#include "../nodeComponents/UiDebugWindow.h"
#include "rendering/RenderManager.h"

#include <iostream>
#include <utility>

#include <GLFW/glfw3.h>

namespace Engine
{
    EngineManager::EngineManager()
        : m_sceneNode(nullptr)
        , m_camera(nullptr)
        , m_lastFrameTimestamp(0)
        , m_deltaTime(0)
        , m_lastFpsCalc(0)
        , m_currentFrameTimestamp(0)
        , m_frames(0)
        , m_fpsCount(0)
        , m_renderManager(nullptr)
    {
        m_renderManager = SingletonManager::get<RenderManager>();
        m_renderManager->init();
    }

    bool EngineManager::engineStart()
    {
        if(!getScene())
        {
            std::cout << "No scene origin node" << std::endl;
            return false;
        }

        m_lastFrameTimestamp = glfwGetTime();

        m_sceneNode->start();

        return true;
    }

    void EngineManager::engineUpdate()
    {
        const auto func = [](BasicNode* node) { node->update(); };

        getScene()->callOnAllChildrenRecursiveAndSelf(func);
    }

    void EngineManager::engineLateUpdate()
    {
        const auto func = [](BasicNode* node) { node->lateUpdate(); };

        getScene()->callOnAllChildrenRecursiveAndSelf(func);
    }

    void EngineManager::engineDraw()
    {
        if(!m_camera)
        {
            fprintf(stderr, "No camera...\n");
            assert(false);
            return;
        }

        m_renderManager->drawScene(m_camera);
    }

    void EngineManager::setScene(std::shared_ptr<BasicNode> sceneNode)
    {
        if(m_sceneNode)
        {
            m_sceneNode->deleteAllChildren();
        }
        m_sceneNode = std::move(sceneNode);
    }

    void EngineManager::setDeltaTime()
    {
        m_currentFrameTimestamp = glfwGetTime();
        m_deltaTime = m_currentFrameTimestamp - m_lastFrameTimestamp;
        m_lastFrameTimestamp = m_currentFrameTimestamp;

        updateFps();
    }

    float EngineManager::getDeltaTime() const { return float(m_deltaTime); }

    void EngineManager::updateFps()
    {
        m_frames++;
        if(m_currentFrameTimestamp - m_lastFpsCalc >= 1)
        {
            m_fpsCount = m_frames;
            m_frames = 0;
            m_lastFpsCalc = m_currentFrameTimestamp;
        }
    }
} // namespace Engine