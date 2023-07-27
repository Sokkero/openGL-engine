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
            TransformComponent() : m_modelMatrix(glm::mat4(1.f)) {};
            ~TransformComponent() = default;

            void moveObj(glm::vec3 dirVec) { m_modelMatrix = glm::translate(m_modelMatrix, dirVec); };
            // TODO: create get/set rotation funcs
            void rotateObj(glm::vec3 dirVec, float degrees) { m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(degrees), dirVec); };

            glm::mat4 getLocalModelMatrix() const { return m_modelMatrix; };
            void setModelMatrix(glm::mat4 matrix) { m_modelMatrix = matrix; };

            glm::vec4 getLocalPosition() const { return m_modelMatrix[3]; };
            void setPosition(glm::vec3 pos) { m_modelMatrix[3] = glm::vec4(pos, 1.f); };

            // TODO: Scaling also scales its global position, no clue how to fix for now
            //  Maybe separate pos, rot and scale and combine them whenever needed?
            glm::vec3 getScale() const
            {
                glm::vec3 scale;
                scale.x = glm::length(glm::vec3(m_modelMatrix[0][0], m_modelMatrix[0][1], m_modelMatrix[0][2]));
                scale.y = glm::length(glm::vec3(m_modelMatrix[1][0], m_modelMatrix[1][1], m_modelMatrix[1][2]));
                scale.z = glm::length(glm::vec3(m_modelMatrix[2][0], m_modelMatrix[2][1], m_modelMatrix[2][2]));
                return scale;
            };
            void setScale(glm::vec3 scale) { m_modelMatrix = glm::scale(m_modelMatrix, scale); };

            glm::vec3 getForward() { return glm::normalize(glm::vec3(m_modelMatrix[0][2], m_modelMatrix[1][2], m_modelMatrix[2][2])); };
            glm::vec3 getBackwards() { return -glm::normalize(glm::vec3(m_modelMatrix[0][2], m_modelMatrix[1][2], m_modelMatrix[2][2])); };
            glm::vec3 getLeft() { return glm::normalize(glm::vec3(m_modelMatrix[0][0], m_modelMatrix[1][0], m_modelMatrix[2][0])); };
            glm::vec3 getRight() { return -glm::normalize(glm::vec3(m_modelMatrix[0][0], m_modelMatrix[1][0], m_modelMatrix[2][0])); };
            glm::vec3 getDown() { return glm::normalize(glm::vec3(m_modelMatrix[0][1], m_modelMatrix[1][1], m_modelMatrix[2][1])); };
            glm::vec3 getUp() { return -glm::normalize(glm::vec3(m_modelMatrix[0][1], m_modelMatrix[1][1], m_modelMatrix[2][1])); };

        protected:
            glm::mat4 m_modelMatrix;
    };

}
