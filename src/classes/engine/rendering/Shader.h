#pragma once

#include "../../nodeComponents/CameraComponent.h"
#include "../../nodeComponents/GeometryComponent.h"
#include "RenderManager.h"
#include "UboBlock.h"
#include <utility>

namespace Engine
{
    inline const GLuint GLOBAL_ATTRIB_INDEX_VERTEXPOSITION = 0;
    inline const GLuint GLOBAL_ATTRIB_INDEX_VERTEXCOLOR = 1;
    inline const GLuint GLOBAL_ATTRIB_INDEX_VERTEXNORMAL = 2;

    class Shader
    {
        public:
            Shader() = default;

            ~Shader()
            {
                // TODO: cehck if this is the correct way to handle expired programms
                GLint numShaders;
                glGetProgramiv(m_shaderIdentifier.second, GL_ATTACHED_SHADERS, &numShaders);

                // Create an array to store the shader object IDs
                auto* shaderIds = new GLuint[numShaders];

                // Get the attached shader primitives
                glGetAttachedShaders(m_shaderIdentifier.second, numShaders, nullptr, shaderIds);

                // Detach and delete the shader primitives if needed
                for(int i = 0; i < numShaders; ++i)
                {
                    GLuint shaderId = shaderIds[i];
                    glDetachShader(m_shaderIdentifier.second, shaderId);
                    glDeleteShader(shaderId);
                }

                // Finally, delete the program
                glDeleteProgram(m_shaderIdentifier.second);
                delete[](shaderIds);
            };

            void registerShader(
                    const std::shared_ptr<RenderManager>& renderManager,
                    const std::string& shaderPath,
                    const std::string& shaderName
            )
            {
                m_shaderIdentifier = renderManager->registerShader(shaderPath, shaderName);
            };

            virtual void renderVertices(std::shared_ptr<GeometryComponent> object, CameraComponent* camera) = 0;

            std::pair<std::string, GLuint> getShaderIdentifier() { return m_shaderIdentifier; }

            GLuint getActiveUniform(const std::string& uniform) const
            {
                const GLuint index = glGetUniformLocation(m_shaderIdentifier.second, uniform.c_str());

                if(index == GL_INVALID_VALUE)
                {
                    fprintf(stderr, "Uniform index not found! Shader invalid");
                    return -1;
                }
                else if(index == GL_INVALID_OPERATION)
                {
                    fprintf(stderr, "Uniform index not found! Linking failed");
                    return -1;
                }

                return index;
            }

            std::vector<std::shared_ptr<UboBlock>> getBoundUbos() { return m_boundUbos; }

            void bindUbo(const std::shared_ptr<UboBlock>& ubo)
            {
                unsigned int index =
                        glGetUniformBlockIndex(m_shaderIdentifier.second, ubo->getBindingPoint().first);

                if(index == GL_INVALID_INDEX)
                {
                    fprintf(stderr, "Ubo index not found!");
                    return;
                }

                glUniformBlockBinding(m_shaderIdentifier.second, index, ubo->getBindingPoint().second);

                m_boundUbos.push_back(ubo);
            }

            void removeBoundUbo(const std::shared_ptr<UboBlock>& ubo)
            {
                m_boundUbos.erase(std::remove(m_boundUbos.begin(), m_boundUbos.end(), ubo), m_boundUbos.end());
            }

            static void bindTexture(
                    GLuint attribId,
                    GLenum targetType,
                    GLuint bufferId,
                    int size,
                    GLenum dataType,
                    bool normalized,
                    int stride,
                    GLenum textureType,
                    GLenum textureTargetType,
                    GLuint textureBufferId,
                    GLint textureSamplerUniformId
            )
            {
                glActiveTexture(textureType);
                glBindTexture(textureTargetType, textureBufferId);
                glUniform1i(textureSamplerUniformId, 0);

                bindVertexData(attribId, targetType, bufferId, size, dataType, normalized, stride);
            }

            static void bindVertexData(
                    GLuint attribId,
                    GLenum targetType,
                    GLuint bufferId,
                    int size,
                    GLenum dataType,
                    bool normalized,
                    int stride
            )
            {
                glEnableVertexAttribArray(attribId);
                glBindBuffer(targetType, bufferId);
                glVertexAttribPointer(attribId, size, dataType, normalized, stride, (void*)nullptr);
            }

        private:
            std::pair<std::string, GLuint> m_shaderIdentifier;
            std::vector<std::shared_ptr<UboBlock>> m_boundUbos;
    };
} // namespace Engine