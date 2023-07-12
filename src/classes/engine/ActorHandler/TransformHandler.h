#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{

    class TransformHandler
    {
        public:
            TransformHandler() : m_modelMatrix(glm::mat4(1.f)) {};
            ~TransformHandler() = default;

            void setPosition(glm::vec3 pos) { m_modelMatrix[3] = glm::vec4(pos, 1.f); };
            void moveObj(glm::vec3 dirVec) { m_modelMatrix = glm::translate(m_modelMatrix, dirVec); };
            void rotateObj(glm::vec3 dirVec, float degrees) { m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(degrees), dirVec); };

            glm::vec4 getPosition() const { return m_modelMatrix[3]; };
            glm::mat4 getModelMatrix() const { return m_modelMatrix; };
            void setModelMatrix(glm::mat4 matrix) { m_modelMatrix = matrix; };

        protected:
            glm::mat4 m_modelMatrix;
    };

}
