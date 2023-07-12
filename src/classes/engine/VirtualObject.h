#pragma once

#include <cstdlib>
#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderManager.h"
#include "ActorHandler/TransformHandler.h"

namespace Engine
{

    class VirtualObject : public TransformHandler
    {
        public:
            explicit VirtualObject(std::shared_ptr<ObjectData>& objectData);
            ~VirtualObject() = default;

            glm::vec4 getTint() const { return m_tint; };
            std::shared_ptr<ObjectData> getObjectData() const { return m_objectData; };
            ShaderType getShader() const { return m_shader; };
            GLuint getTextureBuffer() const { return m_textureBuffer; };

            void setTint(glm::vec4 tint) { m_tint = tint; };
            void setTextureBuffer(GLuint buffer) { m_textureBuffer = buffer; };
            void setShader(ShaderType shader, RenderManager& renderManager);


        private:
            std::shared_ptr<ObjectData> m_objectData;
            GLuint m_matrixId;
            ShaderType m_shader;
            GLuint m_textureBuffer;
            glm::vec4 m_tint;
    };

}
