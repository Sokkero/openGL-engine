#pragma once

#include "classes/SingletonManager.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace Engine
{
    class BasicNode;
    class RenderManager;
    class CameraComponent;
    class RenderComponent;

    namespace Ui
    {
        class UiDebugWindow;
    }

    class EngineManager : public SingletonBase
    {
        public:
            EngineManager();
            ~EngineManager() = default;

            bool engineStart();
            void engineUpdate();
            void engineDraw();
            void engineLateUpdate();

            void setScene(std::shared_ptr<BasicNode> sceneNode);

            std::shared_ptr<BasicNode> getScene() const { return m_sceneNode; };

            void setCamera(std::shared_ptr<CameraComponent> camera) { m_camera = std::move(camera); };

            std::shared_ptr<CameraComponent> getCamera() const { return m_camera; };

            std::shared_ptr<RenderManager> getRenderManager() const { return m_renderManager; };

            void setDeltaTime();
            float getDeltaTime() const;

            void updateFps();

            int getFpsCount() const { return m_fpsCount; };

        private:
            std::shared_ptr<RenderManager> m_renderManager;
            std::shared_ptr<BasicNode> m_sceneNode;
            std::shared_ptr<CameraComponent> m_camera;

            double m_deltaTime;
            double m_currentFrameTimestamp;
            double m_lastFrameTimestamp;
            double m_lastFpsCalc;
            int m_fpsCount;
            int m_frames;
    };

} // namespace Engine
