#pragma once

#include <GL/glew.h>
#include <utility>

namespace Engine
{
    class UboBlock
    {
        public:
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

                UpdateUbo();
            }

            virtual void UpdateUbo() = 0;

            template<typename T>
            void LoadVariable(T data, int byteOffset)
            {
                glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
                glBufferSubData(GL_UNIFORM_BUFFER, byteOffset, sizeof(T), &data);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

            void setBindingPoint(std::pair<char*, GLuint> point) { m_bindingPoint = point; }

            std::pair<char*, GLuint> getBindingPoint() { return m_bindingPoint; }

            void setSize(GLuint size) { m_size = size; }

            GLuint getSize() const { return m_size; }

            GLuint getId() const { return m_uboId; }

        private:
            std::pair<char*, GLuint> m_bindingPoint;
            GLuint m_size = 0;
            GLuint m_uboId = -1;
    };
} // namespace Engine