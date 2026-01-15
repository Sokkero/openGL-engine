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
    class Shader;
    class GridShader;
    class CameraComponent;

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
            void drawNode(const std::shared_ptr<RenderComponent>& node, const std::shared_ptr<CameraComponent>& camera);

            // A custom object will bypass the check if the given file has already been loaded, assuming the code will change the data and make it unique
            std::shared_ptr<ObjectData> registerObject(const char* filePath, bool isCustomObject = false);
            void deregisterObject(std::shared_ptr<ObjectData>& obj);
            void clearObjects();

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

            bool isGridVisible() const { return m_showGrid; };
            void setGridVisibility(bool showGrid) { m_showGrid = showGrid; };

            void setClearColor(const float color[4]);
            float* getClearColor() { return m_clearColor; };

            void addGeometryToScene(std::shared_ptr<RenderComponent>& node);
            void removeGeometryFromScene(const unsigned int& nodeId);

            void addDebugUiToScene(std::shared_ptr<Ui::UiDebugWindow>& node);
            void removeDebugUiFromScene(const unsigned int& nodeId);

            GLuint getDefaultVao() const { return m_defaultVao; }

        private:
            void drawOpaqueNodes(const std::shared_ptr<CameraComponent>& camera);
            void drawTranslucentNodes(const std::shared_ptr<CameraComponent>& camera);
            void drawUiNodes();

            void depthSortNodes(const std::shared_ptr<CameraComponent>& camera);

            std::vector<std::shared_ptr<RenderComponent>> m_sceneGeometry;
            std::vector<std::shared_ptr<Ui::UiDebugWindow>> m_sceneDebugUi;

            std::shared_ptr<UBOs::AmbientLightUbo> m_ambientLightUbo;
            std::shared_ptr<UBOs::DiffuseLightUbo> m_diffuseLightUbo;
            std::shared_ptr<UBOs::ViewProjectionUbo> m_vpUbo;

            std::map<std::string, GLuint> m_shaderList;
            std::vector<std::shared_ptr<ObjectData>> m_objectList;
            std::map<std::string, GLuint> m_textureList;

            std::shared_ptr<GridShader> m_gridShader;

            bool m_showWireframe;
            bool m_showGrid;

            float m_clearColor[4];
            GLuint m_defaultVao;
    };

} // namespace Engine
