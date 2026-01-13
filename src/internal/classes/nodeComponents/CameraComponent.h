#pragma once

#include "BasicNode.h"
#include "TransformComponent.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Engine
{
    /**
     * @brief The CameraComponent class represents a camera component that can be attached to a node in the engine.
     */
    class CameraComponent : virtual public BasicNode
    {
        public:
            CameraComponent();
            ~CameraComponent() = default;

            /**
             * @brief Gets the projection matrix of the camera.
             * @return The projection matrix.
             */
            glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; };

            /**
             * @brief Gets the view matrix of the camera.
             * @return The view matrix.
             */
            glm::mat4 getViewMatrix();

            /**
             * @brief Gets the field of view (FOV) of the camera.
             * @return The field of view.
             */
            float getFov() const { return m_fov; };

            /**
             * @brief Gets the aspect ratio of the camera.
             * @return The aspect ratio.
             */
            float getAspectRatio() const { return m_aspectRatio; };

            /**
             * @brief Gets the near clipping plane of the camera.
             * @return The near clipping plane.
             */
            float getZNear() const { return m_zNear; };

            /**
             * @brief Gets the far clipping plane of the camera.
             * @return The far clipping plane.
             */
            float getZFar() const { return m_zFar; };

            /**
             * @brief Sets the field of view (FOV) of the camera.
             * @param fov The field of view to set.
             */
            void setFov(float fov);

            /**
             * @brief Sets the aspect ratio of the camera.
             * @param aspectRatio The aspect ratio to set.
             */
            void setAspectRatio(float aspectRatio);

            /**
             * @brief Sets the near clipping plane of the camera.
             * @param zNear The near clipping plane to set.
             */
            void setZNear(float zNear);

            /**
             * @brief Sets the far clipping plane of the camera.
             * @param zFar The far clipping plane to set.
             */
            void setZFar(float zFar);

        private:
            /**
             * @brief Updates the projection matrix of the camera.
             */
            void updateProjectionMatrix();

            glm::mat4 m_projectionMatrix;
            float m_fov;
            float m_aspectRatio;
            float m_zNear;
            float m_zFar;
    };

} // namespace Engine
