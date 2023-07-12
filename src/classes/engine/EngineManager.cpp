
#include "EngineManager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    EngineManager::EngineManager()
    : m_sceneObjects(std::vector<VirtualObject*>())
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
        //One color for each vertex. They were generated randomly.
        //static std::vector<glm::vec4>  g_color_buffer_data;
        //for (int v = 0; v < 12*3 ; v++){
            //g_color_buffer_data.emplace_back(.6f, .2f, .2f, 1.f);
        //}

        //virtualObject* newObject1 = virtualObject::newVirtualObjectByFile("resources/objects/cube.obj", g_color_buffer_data, shaderType::solidColor, glm::vec3(0.f, 0.f, 0.f));
        //virtualObject* newObject2 = virtualObject::newVirtualObject(g_vertex_buffer_data, g_color_buffer_data,shaderType::solidColor,  glm::vec3(0.f, 5.f, 0.f));
        //virtualObject* newObject3 = virtualObject::newVirtualObject(g_vertex_buffer_data, g_color_buffer_data,shaderType::solidColor,  glm::vec3(5.f, 0.f, 0.f));

        //newObject1->moveObj(glm::vec3(0.f, 0.f, -5.f));
        //newObject1->rotateObj(glm::vec3(0.f, 1.f, 0.f), 180);

        //addObjectToScene(newObject1);
        //addObjectToScene(newObject2);
        //addObjectToScene(newObject3);

        m_camera = new CameraModel();
        m_camera->setModelMatrix(glm::lookAt(
                glm::vec3(10,10,10), // Camera is at (x,y,z), in World Space
                glm::vec3(0,0,0), // looks at (x,y,z)
                glm::vec3(0,1,0)  // Camera rotation (set to (0,1,0) to look right side up)
        ));

        glClearColor(.7f, .7f, .7f, .0f);

        m_lastFrameTimestamp = glfwGetTime();
    }

    void EngineManager::engineUpdate()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen

        for (auto* object : m_sceneObjects)
        {
            // MVP = Projection * View * Model (Matrix calculations are the other way around)
            glm::mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getModelMatrix() * object->getModelMatrix();
            //const ShaderType& shader = object->getShaderType();
            //const GLuint& vertexBufferId = object->getVertexBuffer();
            //const GLuint& colorBufferId = object->getColorBuffer();
            //const GLuint& matrixId = object->getMatrixID();
            //const int vertexCount = object->getVertexData().size() * 3;

            m_renderManager->renderVertices(object, mvp);
        }
    }

    void EngineManager::clearScene()
    {
        for (auto* object : m_sceneObjects)
        {
            delete(object);
        }
        m_sceneObjects.clear();
    }

    void EngineManager::addObjectToScene(VirtualObject* object)
    {
        m_sceneObjects.push_back(object);
    }

    void EngineManager::removeObjectFromScene(VirtualObject *object)
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
}