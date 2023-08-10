#pragma once

#include "ObjectHelper.h"

#include <glm/gtc/matrix_transform.hpp>
#include <map>

#include <iostream>

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

    struct AmbientLight
    {
        public:
            AmbientLight(const std::map<ShaderType, GLuint>& shaderMap)
            {
                SetupUbo(m_ubo, m_uboSize, shaderMap, AMBIENT_LIGHT_POINT);
                UpdateUbo();
            };

            bool getUseAmbient() const { return m_useAmbient; };

            void setUseAmbient(bool useAmbient)
            {
                m_useAmbient = useAmbient;
                UpdateUbo();
            };

            glm::vec3 getColor() const { return m_color; };

            void setColor(glm::vec3 color)
            {
                m_color = color;
                UpdateUbo();
            };

            float getIntensity() const { return m_intensity; };

            void setIntensity(float intensity)
            {
                m_intensity = intensity;
                UpdateUbo();
            };

            GLuint getUbo() const { return m_ubo; };

        private:
            void UpdateUbo()
            {
                glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &m_useAmbient);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &m_intensity);
                glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(glm::vec3), &m_color);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

            int m_useAmbient = true;
            float m_intensity = 1.f;
            glm::vec3 m_color = glm::vec3(1.f, 1.f, 1.f);
            const unsigned int m_uboSize = 32;
            GLuint m_ubo;
    };

    struct DiffuseLight
    {
        public:
            DiffuseLight(const std::map<ShaderType, GLuint>& shaderMap)
            {
                SetupUbo(m_ubo, m_uboSize, shaderMap, DIFFUSE_LIGHT_POINT);
                UpdateUbo();
            };

            bool getUseDiffuse() const { return m_useDiffuse; };

            void setUseDiffuse(bool useDiffuse)
            {
                m_useDiffuse = useDiffuse;
                UpdateUbo();
            };

            glm::vec3 getDir() const { return m_dir; };

            void setDir(glm::vec3 dir)
            {
                m_dir = dir;
                UpdateUbo();
            };

            glm::vec3 getColor() const { return m_color; };

            void setColor(glm::vec3 color)
            {
                m_color = color;
                UpdateUbo();
            };

            float getIntensity() const { return m_intensity; };

            void setIntensity(float intensity)
            {
                m_intensity = intensity;
                UpdateUbo();
            };

            GLuint getUbo() const { return m_ubo; };

        private:
            void UpdateUbo()
            {
                glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &m_useDiffuse);
                glBufferSubData(GL_UNIFORM_BUFFER, 4, sizeof(float), &m_intensity);
                glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(glm::vec3), &m_dir);
                glBufferSubData(GL_UNIFORM_BUFFER, 32, sizeof(glm::vec3), &m_color);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

            int m_useDiffuse = false;
            glm::vec3 m_dir = glm::vec3(1.f, 1.f, 1.f);
            glm::vec3 m_color = glm::vec3(0.5f, 0.5f, 0.5f);
            float m_intensity = 1.f;
            const unsigned int m_uboSize = 52;
            GLuint m_ubo;
    };
} // namespace Engine