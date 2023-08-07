#pragma once

#include <glm/vec4.hpp>
#include <vector>

namespace Engine
{
    class BasicNode;
    class RenderManager;
    class CameraComponent;

    class EngineManager
    {
        public:
            EngineManager();
            ~EngineManager() = default;

            bool engineStart();
            void engineUpdate();
            void engineDraw();

            void drawNode(BasicNode* node);

            void setScene(std::shared_ptr<BasicNode> sceneNode);

            std::shared_ptr<BasicNode> getScene() const { return m_sceneNode; };

            std::shared_ptr<CameraComponent> getCamera() const { return m_camera; };

            void setCamera(std::shared_ptr<CameraComponent> camera) { m_camera = std::move(camera); };

            std::shared_ptr<RenderManager> getRenderManager() const { return m_renderManager; };

            void setDeltaTime();
            float getDeltaTime();

            void setClearColor(glm::vec4 color) { m_clearColor = color; };

            glm::vec4 getClearColor() const { return m_clearColor; };

            void printFps();

        private:
            std::shared_ptr<RenderManager> m_renderManager;
            std::shared_ptr<BasicNode> m_sceneNode;
            std::shared_ptr<CameraComponent> m_camera;

            double m_deltaTime;
            double m_currentFrameTimestamp;
            double m_lastFrameTimestamp;
            double m_lastFpsPrint;
            int m_frames;
            glm::vec4 m_clearColor;
    };

} // namespace Engine
