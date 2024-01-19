#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace Engine
{
    class BasicNode;
    class RenderManager;
    class CameraComponent;
    class GeometryComponent;
    class GridShader;

    namespace Ui
    {
        class UiDebugWindow;
    }

    class EngineManager
    {
        public:
            EngineManager();
            ~EngineManager() = default;

            bool engineStart();
            void engineUpdate();
            void engineDraw();

            void drawNode(const std::shared_ptr<GeometryComponent>& node);

            void setScene(std::shared_ptr<BasicNode> sceneNode);

            std::shared_ptr<BasicNode> getScene() const { return m_sceneNode; };

            std::shared_ptr<CameraComponent> getCamera() const { return m_camera; };

            void setCamera(std::shared_ptr<CameraComponent> camera) { m_camera = std::move(camera); };

            std::shared_ptr<RenderManager> getRenderManager() const { return m_renderManager; };

            void setDeltaTime();
            float getDeltaTime() const;

            void setClearColor(const float color[4]);

            float* getClearColor() { return m_clearColor; };

            void updateFps();

            int getFpsCount() const { return m_fpsCount; };

            bool isGridVisible() const { return m_showGrid; };

            void setGridVisibility(bool showGrid) { m_showGrid = showGrid; };

            void addGeometryToScene(std::shared_ptr<GeometryComponent>& node);
            void removeGeometryFromScene(std::shared_ptr<GeometryComponent>& node);
            void removeGeometryFromScene(std::shared_ptr<BasicNode>& node);
            void removeGeometryFromScene(BasicNode* node);
            void removeGeometryFromScene(const unsigned int& nodeId);

            void addDebugUiToScene(std::shared_ptr<Ui::UiDebugWindow>& node);
            void removeDebugUiFromScene(std::shared_ptr<Ui::UiDebugWindow>& node);

        private:
            void depthSortNodes();

            void drawSolidNodes();

            void drawTranslucentNodes();

            void drawUiNodes();

            static bool nodeSortingAlgorithm(
                    const std::shared_ptr<GeometryComponent>& a,
                    const std::shared_ptr<GeometryComponent>& b,
                    const glm::vec3& cameraPosition
            );

            std::vector<std::shared_ptr<GeometryComponent>> m_sceneGeometry;
            std::vector<std::shared_ptr<Ui::UiDebugWindow>> m_sceneDebugUi;
            std::shared_ptr<RenderManager> m_renderManager;
            std::shared_ptr<BasicNode> m_sceneNode;
            std::shared_ptr<CameraComponent> m_camera;
            std::shared_ptr<GridShader> m_gridShader;

            bool m_showGrid;
            bool m_showDebugUi;
            double m_deltaTime;
            double m_currentFrameTimestamp;
            double m_lastFrameTimestamp;
            double m_lastFpsCalc;
            int m_fpsCount;
            int m_frames;
            float m_clearColor[4];
    };

} // namespace Engine
