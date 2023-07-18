#pragma once

#include "RenderManager.h"
#include "CameraModel.h"

#include <vector>

#include <GLFW/glfw3.h>

namespace Engine
{
    class BasicActor;

    class EngineManager
    {
            //TODO: Template the functions and figure out how to deal with the vector here
            // Use Interface or common class they inherit from
        public:
            EngineManager();
            ~EngineManager() = default;

            void engineDraw();
            void addObjectToScene(BasicActor* obj) { m_sceneObjects.push_back(obj); };
            void removeObjectFromScene(BasicActor* obj);
            void clearScene();

            CameraModel* getCamera() const { return m_camera; };
            double getDeltaTime() { return glfwGetTime() - m_lastFrameTimestamp; };
        private:
            std::vector<BasicActor*> m_sceneObjects;
            CameraModel* m_camera;
            double m_lastFrameTimestamp;
            int m_totalFramesLastSecond;
            RenderManager* m_renderManager;
    };

}
