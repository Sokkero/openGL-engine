#pragma once

#include "BasicNode.h"
#include "TransformComponent.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Engine
{
    class CameraComponent : virtual public BasicNode
    {
        public:
            CameraComponent();
            ~CameraComponent() = default;

            glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; };

            glm::mat4 getViewMatrix();

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

} // namespace Engine
