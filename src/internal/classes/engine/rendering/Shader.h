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

            std::vector<std::shared_ptr<UboBlock>> getBoundUbos() { return m_boundUbos; }
            void bindUbo(const std::shared_ptr<UboBlock>& ubo);
            void removeBoundUbo(const std::shared_ptr<UboBlock>& ubo);

            static void bindTexture(GLuint attribId, GLuint bufferId, GLuint textureBufferId, GLint textureSamplerUniformId);

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

            void loadVertexBuffer(const std::shared_ptr<ObjectData>& object) const;
            void loadNormalBuffer(const std::shared_ptr<ObjectData>& object) const;
            void loadUVBuffer(const std::shared_ptr<ObjectData>& object) const;

            void loadModelMatrix(const std::shared_ptr<RenderComponent>& object) const;
            void loadTint(const std::shared_ptr<RenderComponent>& object) const;
            void loadTextureBuffer(const std::shared_ptr<RenderComponent>& object) const;
            void loadColorBuffer(const std::shared_ptr<RenderComponent>& object) const;

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