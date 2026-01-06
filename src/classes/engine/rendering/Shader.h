#pragma once

#include "../../nodeComponents/CameraComponent.h"
#include "../../nodeComponents/GeometryComponent.h"
#include "RenderManager.h"
#include "ubos/UboBlock.h"
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
                    const std::string& shaderName
            );

            virtual void renderObject(const std::shared_ptr<GeometryComponent>& object, CameraComponent* camera) = 0;

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

            void loadModelMatrix(const std::shared_ptr<GeometryComponent>& object) const;
            void loadTint(const std::shared_ptr<GeometryComponent>& object) const;
            void loadVertexBuffer(const std::shared_ptr<GeometryComponent>& object) const;
            void loadNormalBuffer(const std::shared_ptr<GeometryComponent>& object) const;
            void loadTextureBuffer(const std::shared_ptr<GeometryComponent>& object) const;
            void loadColorBuffer(const std::shared_ptr<GeometryComponent>& object) const;

            void drawElements(const std::shared_ptr<GeometryComponent>& object) const;

        private:
            static inline const GLuint GLOBAL_ATTRIB_INDEX_VERTEXPOSITION = 0;
            static inline const GLuint GLOBAL_ATTRIB_INDEX_VERTEXCOLOR = 1;
            static inline const GLuint GLOBAL_ATTRIB_INDEX_VERTEXNORMAL = 2;

            static GLuint CURRENT_PROGRAMM;

            std::shared_ptr<DebugModel> m_debugModel;
            std::pair<std::string, GLuint> m_shaderIdentifier;
            std::vector<std::shared_ptr<UboBlock>> m_boundUbos;
    };
} // namespace Engine