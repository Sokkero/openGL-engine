
#include "RenderManager.h"

#include "NodeComponents/GeometryComponent.h"
#include "loadShader.h"
#include "FileLoading.h"

#include <iostream>

namespace Engine {

    RenderManager::RenderManager()
    : m_objectList(std::map<std::string, std::shared_ptr<ObjectData>>())
    , m_shaderList(std::map<ShaderType, GLuint>())
    {
        GLuint tempShaderID;

        tempShaderID = LoadShaders( "resources/shader/solidColor.vert", "resources/shader/solidColor.frag" );
        m_shaderList[ShaderType::solidColor] = tempShaderID;

        tempShaderID = LoadShaders( "resources/shader/solidTexture.vert", "resources/shader/solidTexture.frag" );
        m_shaderList[ShaderType::solidTexture] = tempShaderID;
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
        std::vector<glm::vec2> uvData;

        if(!loadFileOBJ(filePath, vertexData, uvData, vertexNormals))
        {
            return nullptr;
        }

        GLuint vertexBuffer = createVBO(vertexData);
        GLuint uvBuffer = createVBO(uvData);

        std::shared_ptr<ObjectData> newObject = std::make_shared<ObjectData>(filePath, vertexBuffer, uvBuffer, vertexData, uvData, vertexNormals);

        m_objectList[filePath] = newObject;

        return newObject;
    }

    void RenderManager::deregisterObject(std::shared_ptr<ObjectData>& obj)
    {
        std::erase_if(m_objectList, [&obj](const auto& elem) {
            const bool shouldRemove = elem.second == obj;
            if(shouldRemove)
            {
                GLuint buffer[1] = {obj->m_vertexBuffer};
                glDeleteBuffers(1, buffer);
            }
            return shouldRemove;
        });
    }

    void RenderManager::clearObjects()
    {
        for(auto& obj : m_objectList)
        {
            GLuint buffer[1] = {obj.second->m_vertexBuffer};
            glDeleteBuffers(1, buffer);
        }
        m_objectList.clear();
    }

    GLuint RenderManager::registerTexture(const char* filePath)
    {
        for ( auto& object : m_textureList )
        {
            if ( object.first == filePath )
            {
                return object.second;
            }
        }
        unsigned int width, height;
        unsigned char* data;

        if(!loadFileBMP(filePath, width, height, data))
        {
            return -1;
        }

        // TODO: read up what all of this does in more detail
        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        // OpenGL has now copied the data. Free our own version
        delete [] data;

        // Poor filtering...
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Nice trilinear filtering ...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // ... which requires mipmaps. Generate them automatically.
        glGenerateMipmap(GL_TEXTURE_2D);

        // Return the ID of the texture we just created
        m_textureList[filePath] = textureID;
        return textureID;
    }

    void RenderManager::deregisterTexture(GLuint tex)
    {
        std::erase_if(m_textureList, [&tex](const auto& elem) {
            const bool shouldRemove = elem.second == tex;
            if(shouldRemove)
            {
                GLuint buffer[1] = {tex};
                glDeleteBuffers(1, buffer);
            }
            return shouldRemove;
        });
    }

    void RenderManager::renderVertices(GeometryComponent* object, const glm::mat4& mvp)
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
                0,             // No particular reason for 0, but must match the enabled VertexAttribArray
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
                        1,             // No particular reason for 1, but must match the enabled VertexAttribArray
                        4,             // Size
                        GL_FLOAT,      // Type
                        GL_FALSE,      // Normalized?
                        0,             // Stride
                        (void*) nullptr // Array buffer offset
                );
                break;
            case ShaderType::solidTexture:
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, object->getTextureBuffer()); // Correct?
                glUniform1i(int(getUniform(ShaderType::solidTexture, "textureSampler")), 0);

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, object->getObjectData()->m_uvBuffer);
                glVertexAttribPointer(
                        1,                                // attribute. No particular reason for 1, but must match the enabled VertexAttribArray
                        2,                                // size : U+V => 2
                        GL_FLOAT,                         // type
                        GL_FALSE,                         // normalized?
                        0,                                // stride
                        (void*) nullptr                   // array buffer offset
                );
                break;
            case ShaderType::undefined:
                break;
        }

        // Drawing the object
        glDrawArrays(GL_TRIANGLES, 0, object->getObjectData()->getVertexCount());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
}