#pragma once

#include "ObjectHelper.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    inline const GLuint AMBIENT_LIGHT_POINT = 0;
    inline const GLuint DIFFUSE_LIGHTS_POINT = 1;

    inline void SetupUbo(GLuint& ubo, const unsigned int& size , const std::map<ShaderType, GLuint>& shaderMap, const GLuint& shaderPoint, const int& count = 1)
    {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, size * count, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        for(const auto& shader : shaderMap)
        {
            unsigned int index = glGetUniformBlockIndex(shader.second, "AmbientLightBlock");
            glUniformBlockBinding(shader.second, index, shaderPoint);
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, shaderPoint, ubo);
    }

    template<typename T>
    inline void UpdateUbo(GLuint& ubo, T* data, GLuint shaderPoint, const int& offset = 0)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    struct AmbientLight
    {
        public:
            AmbientLight(const std::map<ShaderType, GLuint>& shaderMap)
            {
                SetupUbo(m_ubo, sizeof(*this), shaderMap, AMBIENT_LIGHT_POINT);
            };

            glm::vec3 GetColor() const { return m_color; };
            void SetColor(glm::vec3 color)
            {
                m_color = color;
                UpdateUbo(m_ubo, this, AMBIENT_LIGHT_POINT);
            };

            float GetIntensity() const { return m_intensity; };
            void SetIntensity(float intensity)
            {
                m_intensity = intensity;
                UpdateUbo(m_ubo, this, AMBIENT_LIGHT_POINT);
            };

            GLuint GetUbo() const { return m_ubo; };

        private:
            glm::vec3 m_color = glm::vec3(1.f, 1.f, 1.f);
            float m_intensity = .0f;
            GLuint m_ubo;
    };

    struct DiffuseLight
    {
        public:
            glm::vec3 GetDir() const { return m_dir; };
            void SetDir(glm::vec3 dir){m_dir = dir;};

            glm::vec3 GetColor() const { return m_color; };
            void SetColor(glm::vec3 color){m_color = color;};

            float GetIntensity() const { return m_intensity; };
            void SetIntensity(float intensity){m_intensity = intensity;};

            static void SetDiffuseLights(std::vector<DiffuseLight> lights, const std::map<ShaderType, GLuint>& shaderMap)
            {
                if(lights.empty() && diffuseCount != -1)
                {
                    glDeleteBuffers(1, &ubo);
                    diffuseCount = -1;
                    return;
                }

                if(lights.empty())
                {
                    return;
                }

                if(lights.size() == diffuseCount)
                {
                    UpdateUbo(ubo, &lights, DIFFUSE_LIGHTS_POINT);
                    return;
                }

                diffuseCount = lights.size();
                glDeleteBuffers(1, &ubo);
                SetupUbo(ubo, sizeof(DiffuseLight), shaderMap, DIFFUSE_LIGHTS_POINT, diffuseCount);
            }

        private:
            glm::vec3 m_dir = glm::vec3(1.f, 1.f, 1.f);
            glm::vec3 m_color = glm::vec3(1.f, 1.f, 1.f);
            float m_intensity = .0f;

            static int diffuseCount;
            static GLuint ubo;
    };
    //TODO: this needs a .cpp file to set the static variables, also consider making these actual classes and maybe components?
}