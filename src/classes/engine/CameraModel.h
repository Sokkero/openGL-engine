#pragma once

#include "ActorHandler/TransformHandler.h"

namespace Engine
{

    class CameraModel : public TransformHandler
    {
        public:
            CameraModel();
            ~CameraModel() = default;

            glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; };
            float getFov() const { return m_fov; };
            float getAspectRatio() const { return m_aspectRatio; };
            float getZNear() const { return m_zNear; };
            float getZFar() const { return m_zFar; };

            void setFov(float fov);
            void setAspectRatio(float aspectRatio);
            void setZNear(float zNear);
            void setZFar(float zFar);

        private:
            void updateProjectionMatrix();

            glm::mat4 m_projectionMatrix;
            float m_fov;
            float m_aspectRatio;
            float m_zNear;
            float m_zFar;
    };

}
