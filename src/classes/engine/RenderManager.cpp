
#include "RenderManager.h"

#include "loadShader.h"
#include "FileLoading.h"

#include <iostream>

namespace Engine {

    RenderManager::RenderManager()
    {
        GLuint tempShaderID;

        tempShaderID = LoadShaders( "resources/shader/simpleVertexShader.vert", "resources/shader/simpleFragmentShader.frag" );
        m_shaderList.emplace_back(ShaderType::solidColor, tempShaderID);

        tempShaderID = LoadShaders( "resources/shader/simpleVertexShader.vert", "resources/shader/simpleFragmentShader.frag" );
        m_shaderList.emplace_back(ShaderType::solidTexture, tempShaderID);
        std::cout << "test" << std::endl;
    }

    GLuint RenderManager::getUniform(ShaderType type, const std::string& uniformName)
    {
        for ( auto& shader : m_shaderList )
        {
            if ( shader.first == type)
                return glGetUniformLocation(shader.second, uniformName.c_str());
        }
        return 0;
    }

    std::shared_ptr<ObjectData> RenderManager::registerObject(const char* filePath)
    {
        for ( auto& object : m_objectList )
        {
            if ( object.first == filePath )
            {
                return object.second;
            }
        }

        std::vector<glm::vec3> vertexData, vertexNormals;
        std::vector<glm::vec2> textureData;

        loadFileOBJ(filePath, vertexData, textureData, vertexNormals);

        GLuint vertexBuffer = createVBO(vertexData);

        std::shared_ptr<ObjectData> newObject = std::make_shared<ObjectData>(vertexBuffer, vertexData, vertexNormals);

        m_objectList.emplace_back(filePath, newObject);

        return newObject;
    }

    void RenderManager::deregisterObject(const std::shared_ptr<ObjectData>& obj)
    {
        for(auto& tempObj : m_objectList)
        {
            if( tempObj.second == obj )
            {
                GLuint buffer[1] = {obj->m_vertexBuffer};
                glDeleteBuffers(1, buffer);

                m_objectList.erase(std::remove(m_objectList.begin(), m_objectList.end(), tempObj), m_objectList.end());
            }
        }
    }

    void RenderManager::renderVertices(VirtualObject* object, const glm::mat4& mvp)
    {
        if(object->getShader() == ShaderType::undefined)
        {
            fprintf(stderr, "Object has its shader undefined");
            return;
        }

        bool foundShader = false;
        for ( auto& shader : m_shaderList)
        {
            if ( shader.first == object->getShader())
            {
                glUseProgram(shader.second);
                foundShader = true;
            }
        }

        if(!foundShader)
        {
            fprintf(stderr, "Couldn't find shader for object, shader in question: %s", ShaderTypeToString(object->getShader()).c_str());
            return;
        }

        // Send our transformation to the currently bound rendering, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(int(object->getMatrixId()), 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, object->getObjectData()->m_vertexBuffer);
        glVertexAttribPointer(
                0,             // No particular reason for 0, but must match the layout in the rendering
                3,             // Size
                GL_FLOAT,      // Type
                GL_FALSE,      // Normalized?
                0,             // Stride
                (void*)nullptr // Array buffer offset
        );

        switch (object->getShader()) {
            case ShaderType::solidColor:
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, object->getTextureBuffer());
                glVertexAttribPointer(
                        1,             // No particular reason for 1, but must match the layout in the rendering
                        4,             // Size
                        GL_FLOAT,      // Type
                        GL_FALSE,      // Normalized?
                        0,             // Stride
                        (void*) nullptr // Array buffer offset
                );
                break;
            case ShaderType::solidTexture:
                break;
        }

        // Drawing the object
        glDrawArrays(GL_TRIANGLES, 0, object->getObjectData()->getVertexCount());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    GLuint RenderManager::createVBO(std::vector<glm::vec3> &data)
    {
        int dataSize = data.size() * sizeof(glm::vec3);

        // Identify the vertex buffer
        GLuint vbo;
        // Generate a buffer with our identifier
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Give vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);

        return vbo;
    }

    GLuint RenderManager::createVBO(std::vector<glm::vec4> &data)
    {
        int dataSize = data.size() * sizeof(glm::vec4);

        // Identify the vertex buffer
        GLuint vbo;
        // Generate a buffer with our identifier
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Give vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);

        return vbo;
    }
}