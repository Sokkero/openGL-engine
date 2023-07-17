#pragma once

#include "RenderManager.h"
#include "CameraModel.h"
#include "VirtualObject.h"

#include <vector>

#include <GLFW/glfw3.h>

namespace Engine
{

    class EngineManager
    {
        public:
            EngineManager();
            ~EngineManager() = default;

            void engineDraw();
            void addObjectToScene(VirtualObject* obj);
            void removeObjectFromScene(VirtualObject* obj);
            void clearScene();

            CameraModel* getCamera() const { return m_camera; };
            double getDeltaTime() { return glfwGetTime() - m_lastFrameTimestamp; };
        private:
            std::vector<VirtualObject*> m_sceneObjects;
            CameraModel* m_camera;
            double m_lastFrameTimestamp;
            int m_totalFramesLastSecond;
            RenderManager* m_renderManager;
    };

}
