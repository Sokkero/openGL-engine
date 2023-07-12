#pragma once

#include "RenderManager.h"
#include "CameraModel.h"
#include "VirtualObject.h"

#include <vector>

namespace Engine
{

    class EngineManager
    {
        public:
            EngineManager();
            ~EngineManager() = default;

            void engineUpdate();
            void addObjectToScene(VirtualObject* obj);
            void removeObjectFromScene(VirtualObject* obj);
            void clearScene();

            CameraModel* getCamera() const { return m_camera; };
            //TODO: Add this back
            //double getDeltaTime() { return glfwGetTime() - m_lastTimestamp; };
        private:
            std::vector<VirtualObject*> m_sceneObjects;
            CameraModel* m_camera;
            double m_lastFrameTimestamp;
            int m_totalFramesLastSecond;
            RenderManager* m_renderManager;
    };

}
