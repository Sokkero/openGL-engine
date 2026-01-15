#pragma once

#include "classes/utils/dataContainer/AdditionalShaderDataBase.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class TextureShaderAdditionalData : public Engine::AdditionalShaderDataBase
{
    public:
        TextureShaderAdditionalData();
        ~TextureShaderAdditionalData() = default;

        void* getData() override { return &m_color; }
        GLuint getDataSize() override { return sizeof(glm::vec4); }

        void setColor(glm::vec4 color) { m_color = color; }
        glm::vec4 getColor() const { return m_color; }

    private:
        GLuint m_texture;
};
