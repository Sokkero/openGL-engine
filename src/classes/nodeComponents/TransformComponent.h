#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
    class BasicNode;

    /**
     * @brief The TransformComponent class represents a component that handles the transformation of a node in the engine.
     */
    class TransformComponent
    {
        public:
            TransformComponent()
                : m_modelMatrix(glm::mat4(1.f))
                , m_scale(glm::vec3(1.f))
                , m_position(glm::vec3(0.f))
                , m_rotation(glm::mat4(1.f)) {};
            ~TransformComponent() = default;

            /**
             * @brief Moves the object in the specified direction.
             * @param dirVec The direction vector to move the object.
             */
            void moveObj(glm::vec3 dirVec)
            {
                m_position += dirVec;
                updateModelMatrix();
            };

            /**
             * @brief Rotates the object around the specified axis by the specified angle.
             * @param dirVec The axis vector to rotate the object around.
             * @param degrees The angle in degrees to rotate the object.
             */
            void rotateObj(glm::vec3 dirVec, float degrees)
            {
                m_rotation *= glm::angleAxis(glm::radians(degrees), dirVec);
                updateModelMatrix();
            };

            /**
             * @brief Gets the rotation of the object in degrees.
             * @return The rotation of the object in degrees.
             */
            glm::vec3 getRotation() const { return glm::degrees(glm::eulerAngles(m_rotation)); };

            /**
             * @brief Sets the rotation of the object in degrees.
             * @param rotDegrees The rotation of the object in degrees.
             */
            void setRotation(glm::vec3 rotDegrees)
            {
                glm::vec3 eulerRadians = glm::radians(rotDegrees);
                m_rotation = glm::quat(eulerRadians);
                updateModelMatrix();
            };

            /**
             * @brief Gets the rotation of the object as a quaternion.
             * @return The rotation of the object as a quaternion.
             */
            glm::quat getRotationQuat() const { return m_rotation; };

            /**
             * @brief Sets the rotation of the object as a quaternion.
             * @param quat The rotation of the object as a quaternion.
             */
            void setRotationQuat(glm::quat quat)
            {
                m_rotation = quat;
                updateModelMatrix();
            };

            /**
             * @brief Gets the model matrix of the object.
             * @return The model matrix of the object.
             */
            glm::mat4 getModelMatrix() const { return m_modelMatrix; };

            /**
             * @brief Sets the model matrix of the object.
             * @param matrix The model matrix of the object.
             */
            void setModelMatrix(glm::mat4 matrix) { m_modelMatrix = matrix; };

            /**
             * @brief Gets the position of the object.
             * @return The position of the object.
             */
            glm::vec3 getPosition() const { return m_modelMatrix[3]; };

            /**
             * @brief Sets the position of the object.
             * @param pos The position of the object.
             */
            void setPosition(glm::vec3 pos)
            {
                m_position = pos;
                updateModelMatrix();
            };

            /**
             * @brief Gets the scale of the object.
             * @return The scale of the object.
             */
            glm::vec3 getScale() const { return m_scale; };

            /**
             * @brief Sets the scale of the object.
             * @param scale The scale of the object.
             */
            void setScale(glm::vec3 scale)
            {
                m_scale = scale;
                updateModelMatrix();
            };

        protected:
            glm::mat4 m_modelMatrix;

            glm::vec3 m_position;
            glm::vec3 m_scale;
            glm::quat m_rotation;

            /**
             * @brief Updates the model matrix of the object based on its position, rotation, and scale.
             */
            void updateModelMatrix()
            {
                glm::mat4 posMat = glm::translate(glm::mat4(1.f), m_position);
                glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), m_scale);
                glm::mat4 rotMat = glm::toMat4(m_rotation);

                m_modelMatrix = posMat * rotMat * scaleMat;
            }
    };

} // namespace Engine
