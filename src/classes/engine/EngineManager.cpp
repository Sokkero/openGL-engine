
#include "EngineManager.h"

#include "../../customCode/testScene/TestSceneOrigin.h"
#include "../nodeComponents/CameraComponent.h"
#include "../nodeComponents/GeometryComponent.h"
#include "../nodeComponents/UiDebugWindow.h"
#include "internalResources/shader/GridShader.h"
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
        , m_clearColor { 0.f, 0.f, 0.f, 0.f }
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

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

        m_lastFrameTimestamp = glfwGetTime();

        m_sceneNode->start();

        return true;
    }

    void EngineManager::engineUpdate()
    {
        updateFps();

        const auto func = [](BasicNode* node) { node->update(); };

        getScene()->callOnAllChildrenRecursiveAndSelf(func);
    }

    void EngineManager::engineDraw()
    {
        if(m_camera)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

            if(m_showGrid)
            {
                m_gridShader->renderVertices(nullptr, m_camera.get());
            }

            getScene()->callOnAllChildren(std::bind(&EngineManager::drawNode, this, std::placeholders::_1));
        }
        else
        {
            fprintf(stderr, "No camera...\n");
        }
    }

    void EngineManager::drawNode(BasicNode* node)
    {
        /*
        if(node->getName() == "cameraHolder")
        {
            auto pos = node->getGlobalPosition();
            std::cout << "x: " << pos.x << " y: " << pos.y << " z: " << pos.z << std::endl;
        }
         */

        const auto& geometry = node->getComponent<GeometryComponent>();
        if(geometry)
        {
            geometry->getShader()->renderVertices(geometry, m_camera.get());
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
        m_sceneGeometry.erase(std::remove_if(m_sceneGeometry.begin(), m_sceneGeometry.end(), [nodeId](const auto& childNode)->bool{ return childNode->getNodeId() == nodeId; }), m_sceneGeometry.end());
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
} // namespace Engine