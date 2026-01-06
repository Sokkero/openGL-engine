#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <utility>

namespace Engine
{
    class UboBlock
    {
        public:
            template<typename T>
            static constexpr bool is_glm_type =
                    std::is_same_v<T, glm::vec2> ||
                    std::is_same_v<T, glm::vec3> ||
                    std::is_same_v<T, glm::vec4> ||
                    std::is_same_v<T, glm::ivec2> ||
                    std::is_same_v<T, glm::ivec3> ||
                    std::is_same_v<T, glm::ivec4> ||
                    std::is_same_v<T, glm::mat2> ||
                    std::is_same_v<T, glm::mat3> ||
                    std::is_same_v<T, glm::mat4>;

            template<typename T>
            static constexpr bool is_vec3_type =
                    std::is_same_v<T, glm::vec3> ||
                    std::is_same_v<T, glm::ivec3>;

            UboBlock() = default;
            ~UboBlock() = default;

            void setupUbo()
            {
                if(m_size == 0)
                {
                    fprintf(stderr, "Ubo is missing values!");
                    return;
                }

                glGenBuffers(1, &m_uboId);
                glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
                glBufferData(GL_UNIFORM_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
                glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint.second, m_uboId);

                printf("Created UBO, ID: %u, size: %u, binding point: %u, name: %s\n",
                       m_uboId, m_size, m_bindingPoint.second, m_bindingPoint.first);

                updateUbo();
            }

            virtual void updateUbo() = 0;

            void setSize(GLuint size) { m_size = size; }
            GLuint getSize() const { return m_size; }

            /*
                Load large data types (mat4 > mat3 > mat2 > vec4 > vec2 > bool/float/int) first.

                vec3s are HEAVILY discouraged from usage, since they are padded to be 4 floats.
                To avoid common pitfalls and not waste bytes, use vec4s instead.
                Same is true for booleans. Use integers instead.

                The Key Difference: Size vs Alignment
                In std140 layout:
                Size:
                    - bool, float, int = 4 bytes
                    - vec2 = 8 bytes (2 floats × 4 bytes each)
                    - vec3, vec4 = 16 bytes
                    - mat2 (mat2x2) = 32 bytes (2 columns × 16 bytes, each column padded to vec4)
                    - mat3 (mat3x3) = 48 bytes (3 columns × 16 bytes)
                    - mat4 (mat4x4) = 64 bytes (4 columns × 16 bytes)
                    - mat2x3 = 32 bytes (2 columns × 16 bytes)
                    - mat2x4 = 32 bytes (2 columns × 16 bytes)
                    - mat3x2 = 48 bytes (3 columns × 16 bytes)
                    - mat3x4 = 48 bytes (3 columns × 16 bytes)
                    - mat4x2 = 64 bytes (4 columns × 16 bytes)
                    - mat4x3 = 64 bytes (4 columns × 16 bytes)

                Alignment (where they can START):
                    - bool, float, int = 4-byte alignment (can start at 0, 4, 8, 12, 16, ...)
                    - vec2 = 8-byte alignment (must start at 0, 8, 16, 24, 32, ...)
                    - vec3, vec4 = 16-byte alignment (must start at 0, 16, 32, 48, ...)
                    - All matrices = 16-byte alignment (must start at 0, 16, 32, 48, ...)

                Note: In std140, matrices are stored as arrays of column vectors, where each
                column is padded to vec4 size (16 bytes) regardless of the actual row count.
            */
            template<typename T>
            void LoadBasicVariable(T data, int byteOffset)
            {
                if constexpr (std::is_same_v<T, bool>)
                {
                    static_assert(false, "Use int instead of bool!");
                    return;
                }

                if constexpr (is_glm_type<T>)
                {
                    static_assert(false, "Use LoadGlmVariable for glm types!");
                    return;
                }

                glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
                glBufferSubData(GL_UNIFORM_BUFFER, byteOffset, sizeof(T), &data);
                GLenum err = glGetError();
                if (err != GL_NO_ERROR) {
                    fprintf(stderr, "glBufferSubData error: %d at offset %d\n", err, byteOffset);
                    assert(false);
                }
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

            template<typename T>
            void LoadGlmVariable(T data, int byteOffset)
            {
                if constexpr (!is_glm_type<T>)
                {
                    static_assert(false, "Use LoadBasicVariable for basic types!");
                    return;
                }

                if constexpr (is_vec3_type<T>)
                {
                    static_assert(false, "Never use vec3 types in std140 layouts, use vec4 with padding data instead!");
                    return;
                }

                glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
                glBufferSubData(GL_UNIFORM_BUFFER, byteOffset, sizeof(T), glm::value_ptr(data));
                GLenum err = glGetError();
                if (err != GL_NO_ERROR) {
                    fprintf(stderr, "glBufferSubData error: %d at offset %d\n", err, byteOffset);
                }
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

            void setBindingPoint(std::pair<const char*, GLuint> point) { m_bindingPoint = point; }

            std::pair<const char*, GLuint> getBindingPoint() { return m_bindingPoint; }

            GLuint getId() const { return m_uboId; }

        private:
            std::pair<const char*, GLuint> m_bindingPoint;
            GLuint m_size = 0;
            GLuint m_uboId = -1;
    };
} // namespace Engine