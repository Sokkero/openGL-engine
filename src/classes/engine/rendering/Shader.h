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
            Shader();
            ~Shader();

            enum passVisual
            {
                PASS_NONE = 0,
                PASS_TEXTURE = 1,
                PASS_COLOR = 2
            };

            void registerShader(
                    const std::shared_ptr<RenderManager>& renderManager,
                    const std::string& shaderPath,
                    const std::string& shaderName
            );

            virtual void renderVertices(std::nullptr_t object, CameraComponent* camera);
            virtual void renderVertices(const std::shared_ptr<GeometryComponent>& object, CameraComponent* camera);
            virtual void loadCustomRenderData(CameraComponent* camera) {};
            virtual void loadCustomRenderData(const std::shared_ptr<GeometryComponent>& object, CameraComponent* camera) {
            };

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

            passVisual getVisualPassStyle() const { return m_passVisual; }

            void setVisualPassStyle(passVisual passType) { m_passVisual = passType; }

        private:
            std::vector<GLuint> m_usedAttribArrays;
            passVisual m_passVisual;

            std::pair<std::string, GLuint> m_shaderIdentifier;
            std::vector<std::shared_ptr<UboBlock>> m_boundUbos;
    };
} // namespace Engine