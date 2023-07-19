#pragma once

#include <vector>

namespace Engine
{
    class BasicActor;
    class RenderManager;
    class CameraModel;

    class EngineManager
    {
            //TODO: Template the functions and figure out how to deal with the vector here
            // Use Interface or common class they inherit from
        public:
            EngineManager();
            ~EngineManager() = default;

            void engineUpdate();
            void engineDraw();
            void addObjectToScene(BasicActor* obj);
            void removeObjectFromScene(BasicActor* obj);
            void clearScene();

            CameraModel* getCamera() const { return m_camera; };
            float getDeltaTime();
        private:
            std::vector<BasicActor*> m_sceneObjects;
            CameraModel* m_camera;
            double m_lastFrameTimestamp;
            int m_totalFramesLastSecond;
            RenderManager* m_renderManager;
    };

}
