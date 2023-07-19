#pragma once

#include "NodeComponents/BasicNode.h"
#include "NodeComponents/TransformComponent.h"

namespace Engine
{
    //TODO: this should be an NodeComponents
    class CameraModel : public BasicNode, public TransformComponent
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
            void update() override;
            void updateProjectionMatrix();

            glm::mat4 m_projectionMatrix;
            float m_fov;
            float m_aspectRatio;
            float m_zNear;
            float m_zFar;
    };

}
