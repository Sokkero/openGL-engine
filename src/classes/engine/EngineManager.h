#pragma once

#include <vector>

namespace Engine
{
    class BasicNode;
    class RenderManager;
    class CameraModel;

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

            CameraModel* getCamera() const { return m_camera; };
            RenderManager* getRenderManager() const { return m_renderManager; };
            void setDeltaTime();
            float getDeltaTime();
        private:
            std::shared_ptr<BasicNode> m_sceneNode;
            //TODO: make this a shared_ptr to a CameraHandler that is somewhere in the scenegraph
            CameraModel* m_camera;
            double m_lastFrameTimestamp;
            int m_totalFramesLastSecond;
            RenderManager* m_renderManager;
    };

}
