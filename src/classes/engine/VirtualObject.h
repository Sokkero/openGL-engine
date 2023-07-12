#pragma once

#include "ActorHandler/TransformHandler.h"
#include "ObjectHelper.h"

#include <memory>

#include <GL/glew.h>

namespace Engine
{
    class RenderManager;

    class VirtualObject : public TransformHandler
    {
        public:
            explicit VirtualObject(std::shared_ptr<ObjectData>& objectData);
            ~VirtualObject() = default;

            glm::vec4 getTint() const { return m_tint; };
            std::shared_ptr<ObjectData> getObjectData() const { return m_objectData; };
            ShaderType getShader() const { return m_shader; };
            GLuint getTextureBuffer() const { return m_textureBuffer; };
            GLuint getMatrixId() const { return m_matrixId; };

            void setTint(glm::vec4 tint) { m_tint = tint; };
            void setTextureBuffer(GLuint buffer) { m_textureBuffer = buffer; };
            void setShader(ShaderType shader, RenderManager* renderManager);


        private:
            std::shared_ptr<ObjectData> m_objectData;
            GLuint m_matrixId;
            ShaderType m_shader;
            GLuint m_textureBuffer;
            glm::vec4 m_tint;
    };

}
