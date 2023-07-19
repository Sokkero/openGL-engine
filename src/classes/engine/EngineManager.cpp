
#include "EngineManager.h"

#include "../VirtualObjects/SceneOrigin.h"
#include "CameraModel.h"
#include "RenderManager.h"
#include "NodeComponents/GeometryComponent.h"

#include <GLFW/glfw3.h>

namespace Engine
{
    EngineManager::EngineManager()
    : m_sceneNode(std::shared_ptr<BasicNode>(nullptr))
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

        m_camera = new CameraModel();
        m_camera->setModelMatrix(glm::lookAt(
                glm::vec3(0,10,15), // Camera is at (x,y,z), in World Space
                glm::vec3(0,0,0), // looks at (x,y,z)
                glm::vec3(0,1,0)  // Camera rotation (set to (0,1,0) to look right side up)
        ));

        glClearColor(.7f, .7f, .7f, .0f);

        m_lastFrameTimestamp = glfwGetTime();
    }

    void EngineManager::engineUpdate()
    {
        dynamic_cast<BasicNode*>(m_camera)->update();
        const auto func = [] (BasicNode* node) { node->update(); };

        getScene()->callOnAllNodes(func);
    }

    void EngineManager::engineDraw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen

        getScene()->callOnAllNodes(std::bind(&EngineManager::drawNode, this, std::placeholders::_1));
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
        m_lastFrameTimestamp = glfwGetTime();
    }

    float EngineManager::getDeltaTime()
    {
        return glfwGetTime() - m_lastFrameTimestamp;
    }
}