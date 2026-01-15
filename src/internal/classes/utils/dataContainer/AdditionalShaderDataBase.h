#pragma once

#include <GL/glew.h>

namespace Engine
{
    class AdditionalShaderDataBase
    {
        public:
            AdditionalShaderDataBase() = default;
            virtual ~AdditionalShaderDataBase() = default;

            virtual void* getData() = 0; // Pointer to the raw data
            virtual size_t getDataTypeSize() = 0; // Size of datas type
            virtual GLenum getGlDataType() = 0; // Base Gl type of data (aka GL_FLOAT)
            virtual GLuint getGlTypeAmount() = 0; // Amount of variables (e.g. mat4 = 16x GL_FLOAT)
    };
}