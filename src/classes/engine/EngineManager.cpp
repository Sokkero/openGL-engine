
#include "EngineManager.h"

#include "../../customCode/testScene/TestSceneOrigin.h"
#include "../nodeComponents/CameraComponent.h"
#include "../nodeComponents/GeometryComponent.h"
#include "../nodeComponents/UiDebugWindow.h"
#include "rendering/RenderManager.h"
#include "rendering/Shader.h"

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
    {
        m_renderManager = std::make_shared<RenderManager>();
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
            // MVP = Projection * View * Model (Matrix calculations are the other way around)
            glm::mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() *
                    node->getGlobalModelMatrix();
            geometry->getShader()->renderVertices(geometry, mvp);
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
            m_sceneNode->removeAllChildNodes();
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
} // namespace Engine