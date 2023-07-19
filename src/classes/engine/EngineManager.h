#pragma once

#include <vector>

namespace Engine
{
    class BasicActor;
    class RenderManager;
    class CameraModel;

    class EngineManager
    {
        public:
            EngineManager();
            ~EngineManager() = default;

            void engineUpdate();
            void engineDraw();
            void addObjectToScene(BasicActor* obj);
            void removeObjectFromScene(BasicActor* obj);
            void clearScene();

            CameraModel* getCamera() const { return m_camera; };
            void setDeltaTime();
            float getDeltaTime();
        private:
            //TODO: This should hold a shared pointer to the Scene Origin and that
            // should hold a vector to its children, and they for theirs and so on
            std::vector<BasicActor*> m_sceneObjects;
            //TODO: make this a shared_ptr to a CameraHandler that is somewhere in the scenegraph
            CameraModel* m_camera;
            double m_lastFrameTimestamp;
            int m_totalFramesLastSecond;
            RenderManager* m_renderManager;
    };

}
