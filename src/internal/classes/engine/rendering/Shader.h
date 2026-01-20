#pragma once

#include "classes/engine/rendering/RenderManager.h"
#include "classes/engine/rendering/ubos/UboBlock.h"
#include "classes/nodeComponents/CameraComponent.h"
#include "classes/nodeComponents/RenderComponent.h"

#include <utility>

namespace Engine
{
    class DebugModel;

    class Shader
    {
        public:
            Shader();
            ~Shader();

            void registerShader(
                    const std::shared_ptr<RenderManager>& renderManager,
                    const std::string& shaderPath,
                    const std::string& shaderName,
                    bool requiresAdditionalData,
                    bool requiresTexture
            );

            virtual void renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent* camera) = 0;

            std::pair<std::string, GLuint> getShaderIdentifier() { return m_shaderIdentifier; }

            GLint getActiveUniform(const std::string& uniform) const;

            const std::vector<std::shared_ptr<UboBlock>>& getBoundUbos() { return m_boundUbos; }
            void bindUbo(const std::shared_ptr<UboBlock>& ubo);
            void removeBoundUbo(const std::shared_ptr<UboBlock>& ubo);

            static void bindVertexData(
                    GLuint attribId,
                    GLenum targetType,
                    GLuint bufferId,
                    int size,
                    GLenum dataType,
                    bool normalized,
                    int stride
            );

            void swapToProgramm() const;

            void loadVertexBuffer(GLuint buffer) const;
            void loadNormalBuffer(GLuint buffer) const;
            void loadUVBuffer(GLuint buffer) const;

            void loadModelMatrix(const glm::mat4& modelMatrix) const;
            void loadTextureBuffer(GLuint buffer) const;

            void drawElements(const std::shared_ptr<RenderComponent>& object) const;

            bool requiresAdditionalData() const { return m_requiresAdditionalData; }
            bool requiresTexture() const { return m_requiresTexture; }

        protected:
            std::shared_ptr<DebugModel> m_debugModel;

        private:
            static GLuint CURRENT_PROGRAMM;

            std::pair<std::string, GLuint> m_shaderIdentifier;
            std::vector<std::shared_ptr<UboBlock>> m_boundUbos;

            bool m_requiresAdditionalData;
            bool m_requiresTexture;
    };
} // namespace Engine