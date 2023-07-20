#pragma once

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

            void engineUpdate();
            void engineDraw();

            void drawNode(BasicNode* node);

            void setScene(std::shared_ptr<BasicNode> sceneNode);
            std::shared_ptr<BasicNode> getScene() const { return m_sceneNode; };

            void setCamera(std::shared_ptr<CameraComponent> camera) { m_camera = std::move(camera); };
            std::shared_ptr<CameraComponent> getCamera() const { return m_camera; };

            RenderManager* getRenderManager() const { return m_renderManager; };
            void setDeltaTime();
            float getDeltaTime();
        private:
            std::shared_ptr<BasicNode> m_sceneNode;
            std::shared_ptr<CameraComponent> m_camera;
            double m_lastFrameTimestamp;
            int m_totalFramesLastSecond;
            RenderManager* m_renderManager;
    };

}
