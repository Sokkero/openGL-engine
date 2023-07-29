#pragma once

#include "../ObjectHelper.h"
#include "../RenderManager.h"
#include "BasicNode.h"

#include <memory>

#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <utility>

namespace Engine
{

    class GeometryComponent : virtual public BasicNode
    {
        public:
            explicit GeometryComponent()
                : m_objectData(nullptr)
                , m_shader(ShaderType::undefined)
                , m_textureBuffer(0)
                , m_tint(glm::vec4(0.f, 0.f, 0.f, 0.f))
                , m_matrixId(0)
            {
            }

            ~GeometryComponent() = default;

            glm::vec4 getTint() const { return m_tint; };

            std::shared_ptr<ObjectData> getObjectData() const { return m_objectData; };

            ShaderType getShader() const { return m_shader; };

            GLuint getTextureBuffer() const { return m_textureBuffer; };

            GLuint getMatrixId() const { return m_matrixId; };

            void setObjectData(std::shared_ptr<ObjectData> objData) { m_objectData = std::move(objData); };

            void setTint(glm::vec4 tint) { m_tint = tint; };

            void setTextureBuffer(GLuint buffer) { m_textureBuffer = buffer; };

            void setShader(ShaderType shader, RenderManager* renderManager)
            {
                if(shader == ShaderType::solidTexture && m_objectData->m_vertexUvs.empty())
                {
                    fprintf(stderr, "Object of shaderType solidTexture has no UV data...\n");
                    shader = ShaderType::solidColor;
                }
                m_shader = shader;
                m_matrixId = renderManager->getUniform(shader, "MVP");
            }

        private:
            std::shared_ptr<ObjectData> m_objectData;
            GLuint m_matrixId;
            ShaderType m_shader;
            GLuint m_textureBuffer;
            glm::vec4 m_tint;
    };

} // namespace Engine
