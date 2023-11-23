#pragma once

#include "../engine/rendering/RenderManager.h"
#include "../helper/ObjectHelper.h"
#include "BasicNode.h"

#include <memory>

#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <utility>

namespace Engine
{
    class Shader;

    class GeometryComponent : virtual public BasicNode
    {
        public:
            explicit GeometryComponent()
                : m_objectData(nullptr)
                , m_shader(nullptr)
                , m_textureBuffer(0)
                , m_tint(glm::vec4(1.f, 1.f, 1.f, 1.f))
            {
            }

            ~GeometryComponent() = default;

            glm::vec4 getTint() const { return m_tint; };

            std::shared_ptr<ObjectData> getObjectData() const { return m_objectData; };

            std::shared_ptr<Shader> getShader() const { return m_shader; };

            GLuint getTextureBuffer() const { return m_textureBuffer; };

            void setObjectData(std::shared_ptr<ObjectData> objData) { m_objectData = std::move(objData); };

            void setTint(glm::vec4 tint) { m_tint = tint; };

            void setTextureBuffer(GLuint buffer) { m_textureBuffer = buffer; };

            void setShader(std::shared_ptr<Shader> shader) { m_shader = std::move(shader); }

        private:
            std::shared_ptr<ObjectData> m_objectData;
            std::shared_ptr<Shader> m_shader;
            GLuint m_textureBuffer;
            glm::vec4 m_tint;
    };

} // namespace Engine
