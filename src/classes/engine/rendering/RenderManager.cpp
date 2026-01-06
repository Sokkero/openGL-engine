
#include "RenderManager.h"

#include "../../helper/FileLoadingUtils.h"
#include "../../helper/VertexIndexingUtils.h"
#include "ShaderLoader.h"

#include <iostream>
#include <string>
#include <utility>

namespace Engine
{

    RenderManager::RenderManager()
        : m_objectList(std::map<std::string, std::shared_ptr<ObjectData>>())
        , m_shaderList(std::map<std::string, GLuint>())
        , m_textureList(std::map<std::string, GLuint>())
        , m_ambientLightUbo(nullptr)
        , m_diffuseLightUbo(nullptr)
        , m_showWireframe(false)
    {
        GLint maxBindingPoints;
        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindingPoints);
        std::cout << "Max UBO binding points: " << maxBindingPoints << std::endl;

        m_ambientLightUbo = std::make_shared<UBOs::AmbientLightUbo>();
        m_diffuseLightUbo = std::make_shared<UBOs::DiffuseLightUbo>();
        m_vpUbo = std::make_shared<UBOs::ViewProjectionUbo>();
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

        if(!FileLoadingUtils::loadFileOBJ(filePath, vertexData, uvData, vertexNormals))
        {
            return nullptr;
        }

        std::vector<triData> triIndexData;

        VertexIndexingUtils::indexVBO(vertexData, uvData, vertexNormals, triIndexData);

        GLuint vertexBuffer = !vertexData.empty() ? createBuffer(vertexData) : -1;
        GLuint uvBuffer = !uvData.empty() ? createBuffer(uvData) : -1;
        GLuint normalBuffer = !vertexNormals.empty() ? createBuffer(vertexNormals) : -1;
        GLuint indexBuffer = !triIndexData.empty() ? createBuffer(triIndexData) : -1;

        std::shared_ptr<ObjectData> newObject = std::make_shared<ObjectData>(
                filePath,
                vertexBuffer,
                uvBuffer,
                normalBuffer,
                indexBuffer,
                vertexData,
                uvData,
                vertexNormals,
                triIndexData
        );

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
            GLuint textureId = FileLoadingUtils::loadFileBMP(filePath);
            m_textureList[filePath] = textureId;
            return textureId;
        }
        else if(fileExtension == "dds" || fileExtension == "DDS")
        {
            GLuint textureId = FileLoadingUtils::loadFileDDS(filePath);
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

    std::pair<std::string, GLuint> RenderManager::registerShader(const std::string& shaderPath, std::string shaderName)
    {
        if(m_shaderList.contains(shaderName))
        {
            auto shader = m_shaderList.find(shaderName);
            return *shader;
        }

        std::pair<std::string, GLuint> newShader;
        newShader.first = std::move(shaderName);
        newShader.second = LoadShaders((shaderPath + ".vert").c_str(), (shaderPath + ".frag").c_str());

        m_shaderList.emplace(newShader);

        return newShader;
    }

    void RenderManager::deregisterShader(std::string shaderName /* = "" */, GLuint shaderId /* = -1 */)
    {
        if(shaderName.empty() && shaderId == -1)
        {
            fprintf(stderr, "Deregistering shader failed. No shader specified");
        }

        std::erase_if(
                m_shaderList,
                [&shaderName, &shaderId](const auto& elem)
                { return elem.first == shaderName || elem.second == shaderId; }
        );
    }

    void RenderManager::setWireframeMode(bool toggle)
    {
        if(toggle == m_showWireframe)
        {
            return;
        }

        glPolygonMode(GL_FRONT_AND_BACK, toggle ? GL_LINE : GL_FILL);
        m_showWireframe = toggle;
    }
} // namespace Engine