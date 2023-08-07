#pragma once

#include "ObjectHelper.h"

#include <glm/gtc/matrix_transform.hpp>
#include <map>

namespace Engine
{
    inline const GLuint AMBIENT_LIGHT_POINT = 0;
    inline const GLuint DIFFUSE_LIGHT_POINT = 1;

    inline const char* ShaderPointToString(GLuint point)
    {
        switch(point)
        {
            case 0:
                return "AmbientLightBlock";
            case 1:
                return "DiffuseLightBlock";
            default:
                return "";
        }
    }

    inline void SetupUbo(
            GLuint& ubo,
            const unsigned int& size,
            const std::map<ShaderType, GLuint>& shaderMap,
            const GLuint& shaderPoint
    )
    {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        for(const auto& shader : shaderMap)
        {
            unsigned int index = glGetUniformBlockIndex(shader.second, ShaderPointToString(shaderPoint));
            glUniformBlockBinding(shader.second, index, shaderPoint);
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, shaderPoint, ubo);
    }

    template<typename T>
    inline void UpdateUbo(GLuint& ubo, T* data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    struct AmbientLight
    {
        public:
            AmbientLight(const std::map<ShaderType, GLuint>& shaderMap)
            {
                SetupUbo(m_ubo, sizeof(AmbientLight), shaderMap, AMBIENT_LIGHT_POINT);
                UpdateUbo(m_ubo, this);
            };

            glm::vec3 getColor() const { return m_color; };

            void setColor(glm::vec3 color)
            {
                m_color = color;
                UpdateUbo(m_ubo, this);
            };

            float getIntensity() const { return m_intensity; };

            void setIntensity(float intensity)
            {
                m_intensity = intensity;
                UpdateUbo(m_ubo, this);
            };

            GLuint getUbo() const { return m_ubo; };

        private:
            glm::vec3 m_color = glm::vec3(1.f, 1.f, 1.f);
            float m_intensity = .0f;
            GLuint m_ubo;
    };

    struct DiffuseLight
    {
        public:
            DiffuseLight(const std::map<ShaderType, GLuint>& shaderMap)
            {
                const unsigned int& vec3 = sizeof(glm::vec3);
                const unsigned int& boolean = sizeof(int);
                const unsigned int& flooat = sizeof(float);
                SetupUbo(m_ubo, sizeof(DiffuseLight), shaderMap, DIFFUSE_LIGHT_POINT);
                UpdateUbo(m_ubo, this);
            };

            bool getUseDiffuse() const { return m_useDiffuse; };

            void setDir(bool useDiffuse)
            {
                m_useDiffuse = useDiffuse;
                UpdateUbo(m_ubo, this);
            };

            glm::vec3 getDir() const { return m_dir; };

            void setDir(glm::vec3 dir)
            {
                m_dir = dir;
                UpdateUbo(m_ubo, this);
            };

            glm::vec3 getColor() const { return m_color; };

            void setColor(glm::vec3 color)
            {
                m_color = color;
                UpdateUbo(m_ubo, this);
            };

            float getIntensity() const { return m_intensity; };

            void setIntensity(float intensity)
            {
                m_intensity = intensity;
                UpdateUbo(m_ubo, this);
            };

            GLuint getUbo() const { return m_ubo; };

        private:
            bool m_useDiffuse = true;
            glm::vec3 m_dir = glm::vec3(0.f, 1.f, 1.f);
            glm::vec3 m_color = glm::vec3(1.f, 0.f, 1.f);
            float m_intensity = 1.0f;
            GLuint m_ubo;
    };
} // namespace Engine