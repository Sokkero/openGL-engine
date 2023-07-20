
#include "EngineManager.h"

#include "../VirtualObjects/SceneOrigin.h"
#include "NodeComponents/CameraComponent.h"
#include "RenderManager.h"
#include "NodeComponents/GeometryComponent.h"
#include "NodeComponents/TransformComponent.h"

#include <iostream>

#include <GLFW/glfw3.h>

namespace Engine
{
    EngineManager::EngineManager()
    : m_sceneNode(nullptr)
    , m_camera(nullptr)
    , m_lastFrameTimestamp(0)
    , m_totalFramesLastSecond(0)
    , m_renderManager(nullptr)
    {
        m_renderManager = new RenderManager();

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        m_sceneNode = std::make_shared<SceneOrigin>();

        glClearColor(.7f, .7f, .7f, .0f);

        m_lastFrameTimestamp = glfwGetTime();
    }

    void EngineManager::engineUpdate()
    {
        const auto func = [] (BasicNode* node) { node->update(); };

        getScene()->callOnAllNodes(func);
    }

    void EngineManager::engineDraw()
    {
        if(m_camera)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen
            getScene()->callOnAllNodes(std::bind(&EngineManager::drawNode, this, std::placeholders::_1));
        }
        else
        {
            fprintf(stderr, "No camera...\n");
        }
    }

    void EngineManager::drawNode(BasicNode* node)
    {
        const auto& transform = dynamic_cast<TransformComponent*>(node);
        const auto& geometry = dynamic_cast<GeometryComponent*>(node);
        if(transform && geometry)
        {
            // MVP = Projection * View * Model (Matrix calculations are the other way around)
            glm::mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getModelMatrix() * transform->getModelMatrix();
            m_renderManager->renderVertices(geometry, mvp);
        }
    }

    void EngineManager::setScene(std::shared_ptr<BasicNode> sceneNode)
    {
        m_sceneNode->removeAllChildNodes();
        m_sceneNode = sceneNode;
    }

    void EngineManager::setDeltaTime()
    {
        m_currentFrameTimestamp = glfwGetTime();
        m_deltaTime = m_currentFrameTimestamp - m_lastFrameTimestamp;
        m_lastFrameTimestamp = m_currentFrameTimestamp;
    }

    float EngineManager::getDeltaTime()
    {
        return m_deltaTime;
    }
}