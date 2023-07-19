
#include "EngineManager.h"

#include "../VirtualObjects/TestObject.h"
#include "CameraModel.h"
#include "RenderManager.h"

#include <GLFW/glfw3.h>

namespace Engine
{
    EngineManager::EngineManager()
    : m_sceneObjects(std::vector<BasicActor*>())
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

        //TODO: Figure out where to put all of this shit
        std::vector<glm::vec4>  g_color_buffer_data;
        for (int v = 0; v < 12*3 ; v++){
            g_color_buffer_data.emplace_back(.6f, .2f, .2f, 1.f);
        }

        //m_renderManager->registerObject("resources/objects/cube.obj")
        auto* newObject1 = new TestObject();
        newObject1->setObjectData(m_renderManager->registerObject("resources/objects/cube.obj"));
        newObject1->setShader(ShaderType::solidColor, m_renderManager);
        newObject1->setPosition(glm::vec3(-5, 0, 0));
        newObject1->setTextureBuffer(m_renderManager->createVBO(g_color_buffer_data));
        addObjectToScene(newObject1);

        auto* newObject2 = new TestObject();
        newObject2->setObjectData(m_renderManager->registerObject("resources/objects/cube.obj"));
        newObject2->setShader(ShaderType::solidColor, m_renderManager);
        newObject2->setPosition(glm::vec3(5, 0, 0));
        newObject2->setTextureBuffer(m_renderManager->createVBO(g_color_buffer_data));
        newObject2->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
        addObjectToScene(newObject2);

        m_camera = new CameraModel();
        m_camera->setModelMatrix(glm::lookAt(
                glm::vec3(0,10,15), // Camera is at (x,y,z), in World Space
                glm::vec3(0,0,0), // looks at (x,y,z)
                glm::vec3(0,1,0)  // Camera rotation (set to (0,1,0) to look right side up)
        ));

        glClearColor(.7f, .7f, .7f, .0f);

        m_lastFrameTimestamp = glfwGetTime();
        m_camera->start();
    }

    void EngineManager::engineUpdate()
    {
        dynamic_cast<BasicActor*>(m_camera)->update();
        for (auto* object : m_sceneObjects)
        {
            object->update();
        }
    }

    void EngineManager::engineDraw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen

        for (auto* object : m_sceneObjects)
        {
            const auto& transform = dynamic_cast<TransformHandler*>(object);
            const auto& geometry = dynamic_cast<GeometryHandler*>(object);
            if(transform && geometry)
            {
                // MVP = Projection * View * Model (Matrix calculations are the other way around)
                glm::mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getModelMatrix() * transform->getModelMatrix();
                m_renderManager->renderVertices(geometry, mvp);
            }
        }
    }

    void EngineManager::addObjectToScene(BasicActor* obj)
    {
        m_sceneObjects.push_back(obj);
        obj->start();
    }

    void EngineManager::clearScene()
    {
        for (auto* object : m_sceneObjects)
        {
            delete(object);
        }
        m_sceneObjects.clear();
    }

    void EngineManager::removeObjectFromScene(BasicActor* object)
    {
        int i = 0;
        for (auto* it : m_sceneObjects)
        {
            if ( it == object )
            {
                delete(object);
                m_sceneObjects.erase(m_sceneObjects.begin() + i);
                return;
            }
            i++;
        }
    }

    float EngineManager::getDeltaTime()
    {
        return glfwGetTime() - m_lastFrameTimestamp;
    }
}