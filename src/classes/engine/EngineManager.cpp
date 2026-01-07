
#include "EngineManager.h"

#include "../../customCode/testScene/TestSceneOrigin.h"
#include "../../resources/shader/GridShader.h"
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
        , m_clearColor { 0.f, 0.f, 0.f, 1.f }
        , m_showGrid(true)
        , m_gridShader(nullptr)
    {
        m_renderManager = std::make_shared<RenderManager>();
        m_gridShader = std::make_shared<GridShader>(m_renderManager);
    }

    bool EngineManager::engineStart()
    {
        if(!getScene())
        {
            std::cout << "No scene origin node" << std::endl;
            return false;
        }

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

        RenderUtils::checkForGLError();

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
        if(m_camera)
        {
            m_renderManager->getVpUbo()->updateUbo(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // TODO: Investigate multithreading
            // Multithread tri sorting here

            depthSortNodes();

            drawOpaqueNodes();

            // Wait for threads to finish here

            drawTranslucentNodes();

            if(m_showGrid)
            {
                m_gridShader->renderObject(nullptr, m_camera.get());
            }

            drawUiNodes();
            glDisable(GL_BLEND);

            RenderUtils::checkForGLError();
        }
        else
        {
            fprintf(stderr, "No camera...\n");
            assert(false);
        }
    }

    void EngineManager::depthSortNodes()
    {
        const auto& cameraPos = getCamera()->getGlobalPosition();
        std::sort(
                m_sceneGeometry.begin(),
                m_sceneGeometry.end(),
                [cameraPos](const auto& a, const auto& b)
                { return EngineManager::nodeSortingAlgorithm(a, b, cameraPos); }
        );
    }

    // Sorted by: Opaque objects first, sorted by their shaderID. Translucent objects second, sorted by their distance to the camera.
    bool EngineManager::nodeSortingAlgorithm(
            const std::shared_ptr<GeometryComponent>& a,
            const std::shared_ptr<GeometryComponent>& b,
            const glm::vec3& cameraPosition
    )
    {
        const auto& aTranslucency = a->getIsTranslucent();
        if(aTranslucency != b->getIsTranslucent())
        {
            return !aTranslucency;
        }
        else if(!aTranslucency)
        {
            return a->getShader()->getShaderIdentifier().second < b->getShader()->getShaderIdentifier().second;
        }

        const auto& distanceA = glm::distance(a->getGlobalPosition(), cameraPosition);
        const auto& distanceB = glm::distance(b->getGlobalPosition(), cameraPosition);

        return distanceA > distanceB;
    }

    void EngineManager::drawOpaqueNodes()
    {
        for(auto& node : m_sceneGeometry)
        {
            if(node->getIsTranslucent())
            {
                return;
            }

            drawNode(node);
        }
    }

    void EngineManager::drawTranslucentNodes()
    {
        glEnable(GL_BLEND);
        for(auto& node : m_sceneGeometry)
        {
            if(!node->getIsTranslucent())
            {
                continue;
            }

            node->depthSortTriangles();

            drawNode(node);
        }
    }

    void EngineManager::drawUiNodes()
    {
        for(int i = 0; i < m_sceneDebugUi.size(); i++)
        {
            if(m_sceneDebugUi[i])
            {
                m_sceneDebugUi[i]->drawUi();
            }
        }
    }

    void EngineManager::drawNode(const std::shared_ptr<GeometryComponent>& node)
    {
        if(node)
        {
            node->getShader()->renderObject(node, m_camera.get());
            return;
        }

        const auto& ui = node->getComponent<Ui::UiDebugWindow>();
        if(ui)
        {
            ui->drawUi();
        }
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

    void EngineManager::setClearColor(const float* color)
    {
        m_clearColor[0] = color[0];
        m_clearColor[1] = color[1];
        m_clearColor[2] = color[2];
        m_clearColor[3] = color[3];

        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    }

    void EngineManager::addGeometryToScene(std::shared_ptr<GeometryComponent>& node)
    {
        node->awake();
        m_sceneGeometry.emplace_back(node);
    }

    void EngineManager::removeGeometryFromScene(const unsigned int& nodeId)
    {
        m_sceneGeometry.erase(
                std::remove_if(
                        m_sceneGeometry.begin(),
                        m_sceneGeometry.end(),
                        [nodeId](const auto& childNode) -> bool { return childNode->getNodeId() == nodeId; }
                ),
                m_sceneGeometry.end()
        );
    }

    void EngineManager::removeGeometryFromScene(Engine::BasicNode* node)
    {
        removeGeometryFromScene(node->getNodeId());
    }

    void EngineManager::removeGeometryFromScene(std::shared_ptr<BasicNode>& node)
    {
        removeGeometryFromScene(node->getNodeId());
    }

    void EngineManager::removeGeometryFromScene(std::shared_ptr<GeometryComponent>& node)
    {
        removeGeometryFromScene(node->getNodeId());
    }

    void EngineManager::addDebugUiToScene(std::shared_ptr<Ui::UiDebugWindow>& node)
    {
        m_sceneDebugUi.emplace_back(node);
    }

    void EngineManager::removeDebugUiFromScene(std::shared_ptr<Ui::UiDebugWindow>& node)
    {
        removeDebugUiFromScene(node->getNodeId());
    }

    void EngineManager::removeDebugUiFromScene(const unsigned int& nodeId)
    {
        m_sceneDebugUi.erase(
                std::remove_if(
                        m_sceneDebugUi.begin(),
                        m_sceneDebugUi.end(),
                        [nodeId](const auto& childNode) -> bool { return childNode->getNodeId() == nodeId; }
                ),
                m_sceneDebugUi.end()
        );
    }
} // namespace Engine