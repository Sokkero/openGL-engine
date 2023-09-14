#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    class BasicNode;

    class TransformComponent
    {
        public:
            TransformComponent()
                : m_modelMatrix(glm::mat4(1.f))
                , m_scale(glm::vec3(1.f))
                , m_position(glm::vec3(0.f))
                , m_rotation(glm::mat4(1.f)) {};
            ~TransformComponent() = default;

            void moveObj(glm::vec3 dirVec)
            {
                m_position += dirVec;
                updateModelMatrix();
            };

            // TODO: create get/set rotation funcs
            void rotateObj(glm::vec3 dirVec, float degrees)
            {
                m_rotation = glm::rotate(m_rotation, glm::radians(degrees), dirVec);
                updateModelMatrix();
            };

            glm::mat4 getLocalModelMatrix() const { return m_modelMatrix; };

            void setModelMatrix(glm::mat4 matrix) { m_modelMatrix = matrix; };

            glm::vec3 getLocalPosition() const { return m_position; };

            glm::mat4 getLocalRotation() const { return m_rotation; };

            void setPosition(glm::vec3 pos)
            {
                m_position = pos;
                updateModelMatrix();
            };

            // TODO: Scaling also scales its global position, no clue how to fix for now
            //  Maybe separate pos, rot and scale and combine them whenever needed?
            glm::vec3 getScale() const { return m_scale; };

            void setScale(glm::vec3 scale)
            {
                m_scale = scale;
                updateModelMatrix();
            };

        protected:
            glm::mat4 m_modelMatrix;

            glm::vec3 m_position;
            glm::vec3 m_scale;
            glm::mat4 m_rotation;

            void updateModelMatrix()
            {
                auto posMat = glm::translate(glm::mat4(1.f), m_position);
                auto scaleMat = glm::scale(glm::mat4(1.f), m_scale);

                m_modelMatrix = posMat * m_rotation * scaleMat;
            }
    };

} // namespace Engine
