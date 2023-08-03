
#include "RenderManager.h"

#include "FileLoading.h"
#include "NodeComponents/GeometryComponent.h"
#include "loadShader.h"

#include <iostream>
#include <string>

namespace Engine
{

    RenderManager::RenderManager()
        : m_objectList(std::map<std::string, std::shared_ptr<ObjectData>>())
        , m_shaderList(std::map<ShaderType, GLuint>())
        , m_textureList(std::map<std::string, GLuint>())
        , m_ambientLight(AmbientLight())
    {
        GLuint tempShaderID;

        tempShaderID = LoadShaders("resources/shader/solidColor.vert", "resources/shader/solidColor.frag");
        m_shaderList[ShaderType::solidColor] = tempShaderID;

        tempShaderID = LoadShaders("resources/shader/solidTexture.vert", "resources/shader/solidTexture.frag");
        m_shaderList[ShaderType::solidTexture] = tempShaderID;
    }

    std::shared_ptr<ObjectData> RenderManager::registerObject(const char* filePath)
    {
        for(auto& object : m_objectList)
        {
            if(object.first == filePath)
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

        std::shared_ptr<ObjectData> newObject =
                std::make_shared<ObjectData>(filePath, vertexBuffer, uvBuffer, vertexData, uvData, vertexNormals);

        m_objectList[filePath] = newObject;

        return newObject;
    }

    void RenderManager::deregisterObject(std::shared_ptr<ObjectData>& obj)
    {
        std::erase_if(
                m_objectList,
                [&obj](const auto& elem)
                {
                    const bool shouldRemove = elem.second == obj;
                    if(shouldRemove)
                    {
                        GLuint buffer[1] = { obj->m_vertexBuffer };
                        glDeleteBuffers(1, buffer);
                    }
                    return shouldRemove;
                }
        );
    }

    void RenderManager::clearObjects()
    {
        for(auto& obj : m_objectList)
        {
            GLuint buffer[1] = { obj.second->m_vertexBuffer };
            glDeleteBuffers(1, buffer);
        }
        m_objectList.clear();
    }

    GLuint RenderManager::registerTexture(const char* filePath)
    {
        std::string filePathString = std::string(filePath);
        size_t dotIndex = filePathString.find_last_of('.');
        if(dotIndex == std::string::npos)
        {
            std::cout << "Texture path " << filePath << " broken" << std::endl;
            return -1;
        }

        for(auto& object : m_textureList)
        {
            if(object.first == filePath)
            {
                return object.second;
            }
        }

        std::string fileExtension = filePathString.substr(dotIndex + 1);
        if(fileExtension == "bmp" || fileExtension == "BMP")
        {
            GLuint textureId = loadFileBMP(filePath);
            m_textureList[filePath] = textureId;
            return textureId;
        }
        else if(fileExtension == "dds" || fileExtension == "DDS")
        {
            GLuint textureId = loadFileDDS(filePath);
            m_textureList[filePath] = textureId;
            return textureId;
        }
        else if(fileExtension == "tga" || fileExtension == "TGA")
        {
            // TODO: make it possible to use TGA texture files
        }

        std::cout << "Texture extension of " << filePath << " not valid" << std::endl;
        return -1;
    }

    void RenderManager::deregisterTexture(GLuint tex)
    {
        std::erase_if(
                m_textureList,
                [&tex](const auto& elem)
                {
                    const bool shouldRemove = elem.second == tex;
                    if(shouldRemove)
                    {
                        GLuint buffer[1] = { tex };
                        glDeleteBuffers(1, buffer);
                    }
                    return shouldRemove;
                }
        );
    }

    void RenderManager::clearTextures()
    {
        for(auto& obj : m_textureList)
        {
            GLuint buffer[1] = { obj.second };
            glDeleteBuffers(1, buffer);
        }
        m_objectList.clear();
    }

    void RenderManager::renderVertices(GeometryComponent* object, const glm::mat4& mvp)
    {
        if(object->getShader() == ShaderType::undefined)
        {
            fprintf(stderr, "Object has its shader undefined");
            return;
        }

        const auto& it = m_shaderList.find(object->getShader());
        if(it == m_shaderList.end())
        {
            fprintf(stderr,
                    "Couldn't find shader for object, shader in question: %s",
                    ShaderTypeToString(object->getShader()).c_str());
            return;
        }

        const auto& shader = it->second;
        glUseProgram(shader);

        // Send our transformation to the currently bound rendering, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M
        // part)
        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &mvp[0][0]);

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

        const glm::vec4 tint = object->getTint();
        glUniform4f(glGetUniformLocation(shader, "tintColor"), tint.x, tint.y, tint.z, tint.w);

        switch(object->getShader())
        {
            case ShaderType::solidColor:
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, object->getTextureBuffer());
                glVertexAttribPointer(
                        1,        // No particular reason for 1, but must match the enabled VertexAttribArray
                        4,        // Size
                        GL_FLOAT, // Type
                        GL_FALSE, // Normalized?
                        0,        // Stride
                        (void*)nullptr // Array buffer offset
                );
                break;
            case ShaderType::solidTexture:
                {
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, object->getTextureBuffer()); //
                    glUniform1i(glGetUniformLocation(shader, "textureSampler"), 0);

                    glEnableVertexAttribArray(1);
                    glBindBuffer(GL_ARRAY_BUFFER, object->getObjectData()->m_uvBuffer);
                    glVertexAttribPointer(
                            1, // attribute. No particular reason for 1, but must match the enabled
                            // VertexAttribArray
                            2,             // size : U+V => 2
                            GL_FLOAT,      // type
                            GL_FALSE,      // normalized?
                            0,             // stride
                            (void*)nullptr // array buffer offset
                    );

                    AmbientLight ambient = getAmbientLight();
                    glUniform3f(glGetUniformLocation(shader, "ambientLight.lightColor"), ambient.color.x, ambient.color.y, ambient.color.z);
                    glUniform1f(glGetUniformLocation(shader, "ambientLight.intensity"), ambient.intensity);
                    break;
                }
            case ShaderType::undefined:
                break;
        }

        // Drawing the object
        glDrawArrays(GL_TRIANGLES, 0, object->getObjectData()->getVertexCount());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
} // namespace Engine