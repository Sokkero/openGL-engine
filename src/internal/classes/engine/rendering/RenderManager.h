#pragma once

#include "classes/SingletonManager.h"
#include "classes/engine/rendering/ubos/AmbientLightUbo.h"
#include "classes/engine/rendering/ubos/DiffuseLightUbo.h"
#include "classes/engine/rendering/ubos/ViewProjectionUbo.h"
#include "classes/utils/dataContainer/ObjectData.h"

#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <string>
#include <vector>

namespace Engine
{
    class RenderComponent;
    class DebugDrawManager;
    class Shader;
    class CameraComponent;
    class RenderInstanceGroup;
    class DebugModel;

    namespace Ui
    {
        class UiDebugWindow;
    }

    class RenderManager : public SingletonBase
    {
        public:
            RenderManager();
            ~RenderManager() = default;

            void init();

            void drawScene(const std::shared_ptr<CameraComponent>& camera);
            void drawNode(
                    const std::shared_ptr<RenderComponent>& node,
                    const std::shared_ptr<CameraComponent>& camera
            );

            // A custom object will bypass the check if the given file has already been loaded, assuming the
            // code will change the data and make it unique
            std::shared_ptr<ObjectData> registerObject(const char* filePath, bool isCustomObject = false);
            void deregisterObject(std::shared_ptr<ObjectData>& obj);
            void clearObjects();

            void addRenderObject(std::shared_ptr<RenderComponent>& node);
            void removeRenderObject(const uint32_t& nodeId);

            GLuint registerTexture(const char* filePath);
            void deregisterTexture(GLuint tex);
            void clearTextures();

            std::pair<std::string, GLuint> registerShader(const std::string& shaderPath, std::string shaderName);
            void deregisterShader(std::string shaderName = std::string(), GLuint shaderId = -1);

            std::shared_ptr<UBOs::AmbientLightUbo>& getAmbientLightUbo() { return m_ambientLightUbo; };
            std::shared_ptr<UBOs::DiffuseLightUbo>& getDiffuseLightUbo() { return m_diffuseLightUbo; };
            std::shared_ptr<UBOs::ViewProjectionUbo>& getVpUbo() { return m_vpUbo; };

            void setWireframeMode(bool toggle);
            bool getWireframeMode() const { return m_showWireframe; };

            bool isDebugUiVisible() const { return m_showDebugUi; };
            void setDebugUiVisibility(bool showDebugUi) { m_showDebugUi = showDebugUi; };

            void setClearColor(const float color[4]);
            float* getClearColor() { return m_clearColor; };

            GLuint getDefaultVao() const { return m_defaultVao; }

        private:
            void renderOpaqueNodes(const std::shared_ptr<CameraComponent>& camera);
            void renderTranslucentNodes(const std::shared_ptr<CameraComponent>& camera);

            void renderGroupList(const std::vector<std::shared_ptr<RenderInstanceGroup>>& groups);
            void renderLooseObjects(std::shared_ptr<CameraComponent> camera);

            void depthSortLooseRenderObjects(const std::shared_ptr<CameraComponent>& camera);

            // Render objects that cant be put into instance groups (e.g. translucent, unique, etc)
            std::vector<std::shared_ptr<RenderComponent>> m_looseRenderObjects;
            // Render objects that wont have their data altered often, if at all
            std::vector<std::shared_ptr<RenderInstanceGroup>> m_staticInstanceGroups;
            // Render objects that will have their data altered often
            std::vector<std::shared_ptr<RenderInstanceGroup>> m_dynamicInstanceGroups;
            std::unordered_map<uint32_t, std::shared_ptr<RenderInstanceGroup>> m_nodeIdToGroupMap;

            std::shared_ptr<UBOs::AmbientLightUbo> m_ambientLightUbo;
            std::shared_ptr<UBOs::DiffuseLightUbo> m_diffuseLightUbo;
            std::shared_ptr<UBOs::ViewProjectionUbo> m_vpUbo;

            std::map<std::string, GLuint> m_shaderList;
            std::vector<std::shared_ptr<ObjectData>> m_objectList;
            std::map<std::string, GLuint> m_textureList;

            bool m_showWireframe;
            bool m_showDebugUi;

            float m_clearColor[4];
            GLuint m_defaultVao;

            std::shared_ptr<DebugModel> m_debugModel;
            std::shared_ptr<DebugDrawManager> m_debugDrawManager;
    };

} // namespace Engine
