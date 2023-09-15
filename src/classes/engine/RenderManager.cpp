
#include "RenderManager.h"

#include "FileLoading.h"
#include "LightingHelper.h"
#include "NodeComponents/GeometryComponent.h"
#include "ShaderLoader.h"

#include <iostream>
#include <string>

namespace Engine
{

    RenderManager::RenderManager()
        : m_objectList(std::map<std::string, std::shared_ptr<ObjectData>>())
        , m_shaderList(std::map<std::string, GLuint>())
        , m_textureList(std::map<std::string, GLuint>())
        , m_ambientLight(nullptr)
        , m_diffuseLight(nullptr)
    {
        m_ambientLight = std::make_unique<AmbientLight>();
        m_diffuseLight = std::make_unique<DiffuseLight>();
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
        GLuint normalBuffer = createVBO(vertexNormals);

        std::shared_ptr<ObjectData> newObject =
                std::make_shared<ObjectData>(filePath, vertexBuffer, uvBuffer, normalBuffer, vertexData, uvData, vertexNormals);

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

    std::pair<std::string, GLuint> RenderManager::registerShader(std::string shaderPath, std::string shaderName)
    {
        std::pair<std::string, GLuint> tempShader;
        tempShader.first = shaderName;
        tempShader.second = LoadShaders((shaderPath + ".vert").c_str(), (shaderPath + ".frag").c_str());

        m_ambientLight->setupShader(tempShader);
        m_diffuseLight->setupShader(tempShader);

        m_shaderList[tempShader.first] = tempShader.second;

        return tempShader;
    }

    void RenderManager::deregisterShader(std::string shaderName /* = "" */, GLuint shaderId /* = -1 */)
    {
        if(shaderName.empty() && shaderId == -1)
        {
            fprintf(stderr, "Deregistering shader failed. No shader specified");
        }

        std::erase_if(
                m_shaderList,
                [&shaderName, &shaderId, this](const auto& elem)
                {
                    const bool shouldRemove = elem.first == shaderName || elem.second == shaderId;
                    if(shouldRemove)
                    {
                        std::cout << "Detached & deleted shader " << elem.first << std::endl;
                        deleteShader(elem.second);
                    }
                    return shouldRemove;
                }
        );
    }

    void RenderManager::clearShader()
    {
        for(auto& elem : m_shaderList)
        {
            deleteShader(elem.second);
        }
        m_shaderList.clear();

        std::cout << "Detached & deleted all shader" << std::endl;
    }

    void RenderManager::deleteShader(GLuint programId)
    {
        GLint numShaders;
        glGetProgramiv(programId, GL_ATTACHED_SHADERS, &numShaders);

        // Create an array to store the shader object IDs
        GLuint* shaderIds = new GLuint[numShaders];

        // Get the attached shader objects
        glGetAttachedShaders(programId, numShaders, nullptr, shaderIds);

        // Detach and delete the shader objects if needed
        for(int i = 0; i < numShaders; ++i)
        {
            GLuint shaderId = shaderIds[i];
            glDetachShader(programId, shaderId);
            glDeleteShader(shaderId);
        }

        // Finally, delete the program
        glDeleteProgram(programId);
        delete[](shaderIds);
    }

    void RenderManager::renderVertices(GeometryComponent* object, const glm::mat4& mvp)
    {
        if(object->getShader().empty())
        {
            fprintf(stderr, "Object shader undefined");
            return;
        }

        const auto& it = m_shaderList.find(object->getShader());
        if(it == m_shaderList.end())
        {
            fprintf(stderr,
                    "Couldn't find shader for object, shader in question: %s",
                    object->getShader().c_str());
            return;
        }

        if(!object->getObjectData()->m_vertexBuffer)
        {
            fprintf(stderr, "Object is missing vertices!");
            return;
        }

        if(!object->getObjectData()->m_normalBuffer)
        {
            fprintf(stderr, "Object is missing vertex normals!");
            return;
        }

        const auto& shader = it->second;
        glUseProgram(shader);

        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(VERTEX_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, object->getObjectData()->m_vertexBuffer);
        glVertexAttribPointer(
                VERTEX_POSITION,
                3,             // Size
                GL_FLOAT,      // Type
                GL_FALSE,      // Normalized?
                0,             // Stride
                (void*)nullptr // Array buffer offset
        );

        glEnableVertexAttribArray(VERTEX_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, object->getObjectData()->m_normalBuffer);
        glVertexAttribPointer(
                VERTEX_NORMAL,
                3,             // size
                GL_FLOAT,      // type
                GL_FALSE,      // normalized?
                0,             // stride
                (void*)nullptr // array buffer offset
        );

        GLint uniformLoc = glGetUniformLocation(shader, "tintColor");
        if(uniformLoc != -1)
        {
            const glm::vec4 tint = object->getTint();
            glUniform4f(uniformLoc, tint.x, tint.y, tint.z, tint.w);
        }

        uniformLoc = glGetUniformLocation(shader, "textureSampler");
        if(uniformLoc != -1)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, object->getTextureBuffer()); //
            glUniform1i(uniformLoc, 0);

            glEnableVertexAttribArray(VERTEX_COLOR);
            glBindBuffer(GL_ARRAY_BUFFER, object->getObjectData()->m_uvBuffer);
            glVertexAttribPointer(
                    VERTEX_COLOR,
                    2,             // size : U+V => 2
                    GL_FLOAT,      // type
                    GL_FALSE,      // normalized?
                    0,             // stride
                    (void*)nullptr // array buffer offset
            );
        }
        else
        {
            glEnableVertexAttribArray(VERTEX_COLOR);
            glBindBuffer(GL_ARRAY_BUFFER, object->getTextureBuffer());
            glVertexAttribPointer(
                    VERTEX_COLOR,
                    4,             // Size
                    GL_FLOAT,      // Type
                    GL_FALSE,      // Normalized?
                    0,             // Stride
                    (void*)nullptr // Array buffer offset
            );
        }

        // Drawing the object
        glDrawArrays(GL_TRIANGLES, 0, object->getObjectData()->getVertexCount());
        glDisableVertexAttribArray(VERTEX_POSITION);
        glDisableVertexAttribArray(VERTEX_COLOR);
        glDisableVertexAttribArray(VERTEX_NORMAL);
    }
} // namespace Engine