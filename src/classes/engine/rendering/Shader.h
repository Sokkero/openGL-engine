#pragma once

#include <utility>

#include "../../nodeComponents/GeometryComponent.h"
#include "UboBlock.h"

namespace Engine
{
    class Shader
    {
        public:
            Shader() = default;
            ~Shader() = default;

            virtual void renderVertices(GeometryComponent* object, const glm::mat4& mvp) = 0;

            virtual std::pair<std::string, GLuint> getShaderIdentifier() { return m_shaderIdentifier; }

            virtual void setShaderIdentifier(std::pair<std::string, GLuint> shaderId)
            {
                m_shaderIdentifier = std::move(shaderId);
            }

            virtual std::map<std::string, GLuint> getAllActiveUniforms() { return m_activeUniforms; }

            virtual void addActiveUniform(const char* uniform)
            {
                const GLuint index = glGetUniformLocation(m_shaderIdentifier.second, uniform);

                if(index == GL_INVALID_VALUE) {
                    fprintf(stderr, "Uniform index not found! Shader invalid");
                    return;
                }
                else if(index == GL_INVALID_OPERATION) {
                    fprintf(stderr, "Uniform index not found! Linking failed");
                    return;
                }

                m_activeUniforms.emplace(uniform, index);
            }

            virtual GLuint getActiveUniform(const std::string& uniform)
            {
                const auto& uniformPosition = m_activeUniforms[uniform];

                if(!uniformPosition)
                {
                    return -1;
                }

                return uniformPosition;
            }

            virtual void removeActiveUniform(const std::string& uniform) { m_activeUniforms.erase(uniform); }

            virtual std::vector<std::shared_ptr<UboBlock>> getBoundUbos() { return m_boundUbos; }

            virtual void bindUbo(const std::shared_ptr<UboBlock>& ubo)
            {
                unsigned int index = glGetUniformBlockIndex(m_shaderIdentifier.second, ubo->getBindingPoint().first);

                if(index == GL_INVALID_INDEX) {
                    fprintf(stderr, "Ubo index not found!");
                    return;
                }

                glUniformBlockBinding(m_shaderIdentifier.second, index, ubo->getBindingPoint().second);

                m_boundUbos.push_back(ubo);
            }

            virtual void removeBoundUbo(const std::shared_ptr<UboBlock>& ubo)
            {
                m_boundUbos.erase(
                        std::remove(m_boundUbos.begin(), m_boundUbos.end(), ubo),
                        m_boundUbos.end()
                );
            }

        private:
            std::pair<std::string, GLuint> m_shaderIdentifier;
            std::map<std::string, GLuint> m_activeUniforms;
            std::vector<std::shared_ptr<UboBlock>> m_boundUbos;
    };
} // namespace Engine