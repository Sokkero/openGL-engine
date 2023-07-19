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

            glm::vec3 getForward() { return glm::normalize(glm::vec3(glm::inverse(m_modelMatrix)[2])); };
            glm::vec3 getBackwards() { return glm::normalize(glm::vec3(glm::inverse(m_modelMatrix)[2]) * glm::vec3(-1.f, -1.f, -1.f)); };
            //glm::vec3 getLeft() { return glm::normalize(glm::vec3(glm::inverse(m_modelMatrix)[2]) * glm::vec3(-1.f, -1.f, -1.f)); };
            //glm::vec3 getRight() { return glm::normalize(glm::vec3(glm::inverse(m_modelMatrix)[2]) * glm::vec3(-1.f, -1.f, -1.f)); };

            glm::vec4 getPosition() const { return m_modelMatrix[3]; };
            glm::vec3 getScale() const { return m_scale; };
            glm::mat4 getModelMatrix() const { return m_modelMatrix; };
            void setModelMatrix(glm::mat4 matrix) { m_modelMatrix = matrix; };

        protected:
            glm::mat4 m_modelMatrix;
            glm::vec3 m_scale;
    };

}
