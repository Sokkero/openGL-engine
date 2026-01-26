#pragma once

#include "classes/utils/dataContainer/AdditionalShaderDataBase.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class ColorShaderAdditionalData : public Engine::AdditionalShaderDataBase
{
    public:
        ColorShaderAdditionalData(const glm::vec4& color) : m_color(color) {};
        ~ColorShaderAdditionalData() = default;

        void* getData() override { return &m_color; }
        size_t getDataTypeSize() override { return sizeof(glm::vec4); }
        GLenum getGlDataType() override { return GL_FLOAT; };
        GLuint getGlTypeAmount() override { return 4; };
        void loadDataAsUniform(GLuint uniLoc) override { glUniform4fv(uniLoc, 1, glm::value_ptr(m_color)); }

        void setColor(glm::vec4 color) { m_color = color; }
        glm::vec4 getColor() const { return m_color; }

    private:
        glm::vec4 m_color;
};
