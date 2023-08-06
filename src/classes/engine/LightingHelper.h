#pragma once

#include "ObjectHelper.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    inline const GLuint AMBIENT_LIGHT_POINT = 0;

    struct AmbientLight
    {
        AmbientLight(const std::map<ShaderType, GLuint>& shaderMap)
        {
            // Setup Ambient lighting
            glGenBuffers(1, &ubo);
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(AmbientLight), this, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            for(const auto& shader : shaderMap)
            {
                unsigned int index = glGetUniformBlockIndex(shader.second, "AmbientLightBlock");
                glUniformBlockBinding(shader.second, index, AMBIENT_LIGHT_POINT);
            }

            glBindBufferBase(GL_UNIFORM_BUFFER, AMBIENT_LIGHT_POINT, ubo);
        };

        void UpdateUbo() const
        {
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(AmbientLight), this);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
        float intensity = .0f;
        GLuint ubo;
    };

    struct DiffuseLight
    {
        glm::vec3 dir = glm::vec3(1.f, 1.f, 1.f);
        glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
        float intensity = .0f;
        GLuint ubo;
    };
}