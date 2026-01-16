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
            TransformComponent();
            ~TransformComponent() = default;

            /**
             * @brief Moves the object in the specified direction.
             * @param dirVec The direction vector to move the object.
             */
            void moveObj(glm::vec3 dirVec);

            /**
             * @brief Rotates the object around the specified axis by the specified angle.
             * @param dirVec The axis vector to rotate the object around.
             * @param degrees The angle in degrees to rotate the object.
             */
            void rotateObj(glm::vec3 dirVec, float degrees);

            /**
             * @brief Gets the rotation of the object in degrees.
             * @return The rotation of the object in degrees.
             */
            glm::vec3 getRotation() const { return glm::degrees(glm::eulerAngles(m_rotation)); }

            /**
             * @brief Sets the rotation of the object in degrees.
             * @param rotDegrees The rotation of the object in degrees.
             */
            void setRotation(glm::vec3 rotDegrees);

            /**
             * @brief Gets the rotation of the object as a quaternion.
             * @return The rotation of the object as a quaternion.
             */
            glm::quat getRotationQuat() const { return m_rotation; }

            /**
             * @brief Sets the rotation of the object as a quaternion.
             * @param quat The rotation of the object as a quaternion.
             */
            void setRotationQuat(glm::quat quat);

            /**
             * @brief Gets the position of the object.
             * @return The position of the object.
             */
            glm::vec3 getPosition() const { return m_position; }

            /**
             * @brief Sets the position of the object.
             * @param pos The position of the object.
             */
            void setPosition(glm::vec3 pos);

            /**
             * @brief Gets the scale of the object.
             * @return The scale of the object.
             */
            glm::vec3 getScale() const { return m_scale; }

            /**
             * @brief Sets the scale of the object.
             * @param scale The scale of the object.
             */
            void setScale(glm::vec3 scale);

            bool getIsDirty() const { return m_isDirty; }
            void setIsDirty(bool isDirty) { m_isDirty = isDirty; }

        protected:
            /**
             * @brief Marks transform data as dirty.
             */
            void setCachedGlobalModelMatrix(const glm::mat4& matrix) { m_globalModelMatrix = matrix; }

            /**
             * @brief Gets the model matrix of the object.
             * @return The model matrix of the object.
             */
            glm::mat4 getCachedGlobalModelMatrix() const { return m_globalModelMatrix; }

            /**
             * @brief Gets the model matrix of the object.
             * @return The model matrix of the object.
             */
            glm::mat4 getLocalModelMatrix() const;

        private:
            glm::mat4 m_globalModelMatrix;
            bool m_isDirty;

            glm::vec3 m_position;
            glm::vec3 m_scale;
            glm::quat m_rotation;
    };

    /*
         void decompose(const glm::mat4& transform, glm::vec3& position, glm::quat& rotation, glm::vec3& scale)
        {
         // Extract position (translation is in the 4th column)
         position = glm::vec3(transform[3]);

        // Extract the 3x3 rotation/scale matrix
        glm::mat3 rotScale = glm::mat3(transform);

        // Extract scale (length of each basis vector)
        scale.x = glm::length(glm::vec3(rotScale[0]));
        scale.y = glm::length(glm::vec3(rotScale[1]));
        scale.z = glm::length(glm::vec3(rotScale[2]));

        // Remove scale from the rotation matrix
        glm::mat3 rotationMatrix;
        rotationMatrix[0] = rotScale[0] / scale.x;
        rotationMatrix[1] = rotScale[1] / scale.y;
        rotationMatrix[2] = rotScale[2] / scale.z;

        // Convert rotation matrix to quaternion
        rotation = glm::quat_cast(rotationMatrix);
        }
     */

} // namespace Engine
