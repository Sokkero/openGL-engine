#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{

    class TransformComponent
    {
        public:
            TransformComponent() : m_modelMatrix(glm::mat4(1.f)) {};
            ~TransformComponent() = default;

            void setPosition(glm::vec3 pos) { m_modelMatrix[3] = glm::vec4(pos, 1.f); };
            void setScale(glm::vec3 scale) { glm::scale(m_modelMatrix, scale); m_scale = scale; };
            void moveObj(glm::vec3 dirVec) { m_modelMatrix = glm::translate(m_modelMatrix, dirVec); };
            void rotateObj(glm::vec3 dirVec, float degrees) { m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(degrees), dirVec); };

            glm::vec3 getForward() { return glm::normalize(glm::vec3(m_modelMatrix[0][2], m_modelMatrix[1][2], m_modelMatrix[2][2])); };
            glm::vec3 getBackwards() { return -glm::normalize(glm::vec3(m_modelMatrix[0][2], m_modelMatrix[1][2], m_modelMatrix[2][2])); };
            glm::vec3 getLeft() { return glm::normalize(glm::vec3(m_modelMatrix[0][0], m_modelMatrix[1][0], m_modelMatrix[2][0])); };
            glm::vec3 getRight() { return -glm::normalize(glm::vec3(m_modelMatrix[0][0], m_modelMatrix[1][0], m_modelMatrix[2][0])); };
            glm::vec3 getDown() { return glm::normalize(glm::vec3(m_modelMatrix[0][1], m_modelMatrix[1][1], m_modelMatrix[2][1])); };
            glm::vec3 getUp() { return -glm::normalize(glm::vec3(m_modelMatrix[0][1], m_modelMatrix[1][1], m_modelMatrix[2][1])); };

            //TODO: create getGlobalX functions
            glm::vec4 getLocalPosition() const { return m_modelMatrix[3]; };
            //TODO: apply scale to rendering
            glm::vec3 getLocalScale() const { return m_scale; };
            glm::mat4 getLocalModelMatrix() const { return m_modelMatrix; };
            void setModelMatrix(glm::mat4 matrix) { m_modelMatrix = matrix; };

        protected:
            glm::mat4 m_modelMatrix;
            glm::vec3 m_scale;
    };

}
