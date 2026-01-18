#include "RenderManager.h"

#include "classes/engine/rendering/ShaderLoader.h"
#include "classes/utils/FileLoadingUtils.h"
#include "classes/utils/VertexIndexingUtils.h"
#include "classes/nodeComponents/UiDebugWindow.h"
#include "resources/shader/gridShader/GridShader.h"
#include "classes/nodeComponents/RenderComponent.h"
#include "classes/engine/rendering/RenderInstanceGroup.h"
#include "classes/engine/WindowManager.h"

#include <iostream>
#include <string>
#include <utility>

namespace Engine
{
    std::atomic<uint32_t> ObjectData::ID_COUNTER = 0;

    RenderManager::RenderManager()
        : m_ambientLightUbo(nullptr)
        , m_diffuseLightUbo(nullptr)
        , m_vpUbo(nullptr)
        , m_showWireframe(false)
        , m_clearColor { 0.f, 0.f, 0.f, 1.f }
        , m_showGrid(true)
        , m_gridShader(nullptr)
    {
    }

    void RenderManager::init()
    {
        GLint maxBindingPoints;
        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindingPoints);
        std::cout << "Max UBO binding points: " << maxBindingPoints << std::endl;

        GLint maxAttribs;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
        std::cout << "Max vertex attributes: " << maxAttribs << std::endl;

        glGenVertexArrays(1, &m_defaultVao);
        glBindVertexArray(m_defaultVao);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

        m_ambientLightUbo = std::make_shared<UBOs::AmbientLightUbo>();
        m_diffuseLightUbo = std::make_shared<UBOs::DiffuseLightUbo>();
        m_vpUbo = std::make_shared<UBOs::ViewProjectionUbo>();
        m_gridShader = std::make_shared<GridShader>();

        RenderUtils::checkForGLError();
    }

    void RenderManager::drawScene(const std::shared_ptr<CameraComponent>& camera)
    {
        m_vpUbo->updateUbo(camera->getViewMatrix(), camera->getProjectionMatrix());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: Investigate multithreading
        // Multithread tri sorting here

        renderGroupList(m_staticInstanceGroups);
        renderGroupList(m_dynamicInstanceGroups);
        
        depthSortLooseRenderObjects(camera);
        renderLooseObjects(camera);

        if(m_showGrid)
        {
            m_gridShader->renderObject(nullptr, camera.get());
        }

        renderUiNodes();
        glDisable(GL_BLEND);

        RenderUtils::checkForGLError();
    }

    void RenderManager::depthSortLooseRenderObjects(const std::shared_ptr<CameraComponent>& camera)
    {
        const auto& cameraPos = camera->getGlobalPosition();
        std::sort(
                m_looseRenderObjects.begin(),
                m_looseRenderObjects.end(),
                [cameraPos](const auto& a, const auto& b)
                { return RenderUtils::nodeDistanceSortingAlgorithm(a, b, cameraPos); }
        );
    }

    void RenderManager::renderGroupList(const std::vector<std::shared_ptr<RenderInstanceGroup>>& groups)
    {
        for(const auto& group : groups)
        {
            group->renderGroup();
        }
    }

    void RenderManager::renderLooseObjects(std::shared_ptr<CameraComponent> camera) 
    {
        renderOpaqueNodes(camera);
        renderTranslucentNodes(camera);
    }

    void RenderManager::renderOpaqueNodes(const std::shared_ptr<CameraComponent>& camera)
    {
        for(auto& node : m_looseRenderObjects)
        {
            if(node->getIsTranslucent())
            {
                return;
            }

            drawNode(node, camera);
        }
    }

    void RenderManager::renderTranslucentNodes(const std::shared_ptr<CameraComponent>& camera)
    {
        glEnable(GL_BLEND);
        for(auto& node : m_looseRenderObjects)
        {
            if(!node->getIsTranslucent())
            {
                continue;
            }

            node->depthSortTriangles();

            drawNode(node, camera);
        }
    }

    void RenderManager::renderUiNodes()
    {
        for(int i = 0; i < m_sceneDebugUi.size(); i++)
        {
            if(m_sceneDebugUi[i])
            {
                m_sceneDebugUi[i]->drawUi();
            }
        }
    }

    void RenderManager::drawNode(
            const std::shared_ptr<RenderComponent>& node,
            const std::shared_ptr<CameraComponent>& camera
    )
    {
        if(node)
        {
            node->getShader()->renderObject(node, camera.get());
            return;
        }

        const auto& ui = node->getComponent<Ui::UiDebugWindow>();
        if(ui)
        {
            ui->drawUi();
        }
    }

    void RenderManager::setClearColor(const float* color)
    {
        m_clearColor[0] = color[0];
        m_clearColor[1] = color[1];
        m_clearColor[2] = color[2];
        m_clearColor[3] = color[3];

        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    }

    void RenderManager::addRenderObject(std::shared_ptr<RenderComponent>& node)
    {
        const auto funcAddToType = [this, node](std::vector<std::shared_ptr<RenderInstanceGroup>>& groups)
        {
            for(const auto& group : groups)
            {
                if(group->addToGroup(node))
                {
                    m_nodeIdToGroupMap[node->getNodeId()] = group;
                    return;
                }
            }

            std::shared_ptr<RenderInstanceGroup> newGroup = std::make_shared<RenderInstanceGroup>(node);
            groups.push_back(newGroup);
        };

        if(node->getIsTranslucent() && node->getRenderType() != RenderTypeEnum::Loose)
        {
            fprintf(stderr, "Translucent nodes have to be of render type loose!");
            node->setRenderType(RenderTypeEnum::Loose);
        }

        RenderTypeEnum renderType = node->getRenderType();
        if(renderType == RenderTypeEnum::Loose)
        {
            m_looseRenderObjects.push_back(node);
        }
        else if(renderType == RenderTypeEnum::Dynamic)
        {
            funcAddToType(m_dynamicInstanceGroups);
        }
        else if(renderType == RenderTypeEnum::Static)
        {
            funcAddToType(m_staticInstanceGroups);
        }

        node->setIsActiveInScene(true);
        node->awake(); // At the end
    }

    void RenderManager::removeRenderObject(const uint32_t& nodeId)
    {
        auto it = m_nodeIdToGroupMap.find(nodeId);
        if(it == m_nodeIdToGroupMap.end())
        {
            m_looseRenderObjects.erase(
                    std::remove_if(
                            m_looseRenderObjects.begin(),
                            m_looseRenderObjects.end(),
                            [nodeId](auto& obj) -> bool
                            {
                                if(obj->getNodeId() == nodeId)
                                {
                                    obj->setIsActiveInScene(false);
                                    return true;
                                }
                                return false;
                            }
                    ),
                    m_looseRenderObjects.end()
            );
            return;
        }

        it->second->removeFromGroup(nodeId);
        m_nodeIdToGroupMap.erase(nodeId);
    }

    void RenderManager::addDebugUiToScene(std::shared_ptr<Ui::UiDebugWindow>& node)
    {
        m_sceneDebugUi.emplace_back(node);
    }

    void RenderManager::removeDebugUiFromScene(const unsigned int& nodeId)
    {
        m_sceneDebugUi.erase(
                std::remove_if(
                        m_sceneDebugUi.begin(),
                        m_sceneDebugUi.end(),
                        [nodeId](const auto& childNode) -> bool { return childNode->getNodeId() == nodeId; }
                ),
                m_sceneDebugUi.end()
        );
    }

    std::shared_ptr<ObjectData> RenderManager::registerObject(const char* filePath, bool isCustomObject /* = false */)
    {
        if(!isCustomObject)
        {
            for(const auto& object : m_objectList)
            {
                if(object->filePath == filePath)
                {
                    return object;
                }
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

        GLuint vertexBuffer = !vertexData.empty() ? RenderUtils::createVertexBufferObject(vertexData) : -1;
        GLuint uvBuffer = !uvData.empty() ? RenderUtils::createVertexBufferObject(uvData) : -1;
        GLuint normalBuffer = !vertexNormals.empty() ? RenderUtils::createVertexBufferObject(vertexNormals) : -1;
        GLuint indexBuffer = !triIndexData.empty() ? RenderUtils::createVertexBufferObject(triIndexData) : -1;

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

        m_objectList.emplace_back(newObject);
        return newObject;
    }

    void RenderManager::deregisterObject(std::shared_ptr<ObjectData>& obj)
    {
        std::erase_if(
                m_objectList,
                [&obj](const auto& elem)
                {
                    const bool shouldRemove = elem == obj;
                    if(shouldRemove)
                    {
                        const std::vector<GLuint> buffers = obj->getActiveBuffers();
                        glDeleteBuffers((int)buffers.size(), &buffers[0]);
                    }
                    return shouldRemove;
                }
        );
    }

    void RenderManager::clearObjects()
    {
        for(auto& obj : m_objectList)
        {
            const std::vector<GLuint> buffers = obj->getActiveBuffers();
            glDeleteBuffers((int)buffers.size(), &buffers[0]);
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
            fprintf(stderr, "Deregistering shader failed. No shader specified\n");
            assert(false);
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