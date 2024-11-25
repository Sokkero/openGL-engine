#pragma once

#include "../engine/EngineManager.h"
#include "../engine/rendering/RenderManager.h"
#include "../helper/ObjectData.h"
#include "BasicNode.h"
#include "CameraComponent.h"

#include <memory>

#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <utility>

namespace Engine
{
    class Shader;

    /**
     * @brief The GeometryComponent class represents a component that handles geometry-related operations for a node.
     */
    class GeometryComponent : virtual public BasicNode
    {
        public:
            explicit GeometryComponent()
                : m_objectData(nullptr)
                , m_shader(nullptr)
                , m_textureBuffer(0)
                , m_tint(glm::vec4(1.f, 1.f, 1.f, 1.f))
                , m_isTranslucent(false)
                , m_customIndexBuffer(0)
                , m_customVertexIndices(std::vector<triData>())
            {
                setIsTranslucent(m_tint.w < 1.f);
            }

            ~GeometryComponent() = default;

            /**
             * @brief Get the tint color of the geometry.
             * @return The tint color as a glm::vec4.
             */
            glm::vec4 getTint() const { return m_tint; };

            /**
             * @brief Set the tint color for the geometry.
             * @param tint The tint color as a glm::vec4.
             */
            void setTint(glm::vec4 tint)
            {
                m_tint = tint;
                setIsTranslucent(m_tint.w < 1.f);
            };

            /**
             * @brief Get the object data associated with the geometry.
             * @return A shared pointer to the ObjectData.
             */
            std::shared_ptr<ObjectData> getObjectData() const { return m_objectData; };

            /**
             * @brief Set the object data for the geometry.
             * @param objData A shared pointer to the ObjectData.
             */
            void setObjectData(std::shared_ptr<ObjectData> objData) { m_objectData = std::move(objData); };

            /**
             * @brief Get the texture buffer ID associated with the geometry.
             * @return The texture buffer ID as a GLuint.
             */
            GLuint getTextureBuffer() const { return m_textureBuffer; };

            /**
             * @brief Set the texture buffer ID for the geometry.
             * @param buffer The texture buffer ID as a GLuint.
             */
            void setTextureBuffer(GLuint buffer) { m_textureBuffer = buffer; };

            /**
             * @brief Get the shader used for rendering the geometry.
             * @return A shared pointer to the Shader.
             */
            std::shared_ptr<Shader> getShader() const { return m_shader; };

            /**
             * @brief Set the shader for rendering the geometry.
             * @param shader A shared pointer to the Shader.
             */
            void setShader(std::shared_ptr<Shader> shader) { m_shader = std::move(shader); }

            /**
             * @brief Get wether or not the Geometry is translucent or not.
             * @return A boolean indicating the translucency.
             */
            bool getIsTranslucent() const { return m_isTranslucent; };

            /**
             * @brief Set wether or not the Geometry is translucent or not.
             * @param bool A boolean setting the translucency.
             */
            void setIsTranslucent(bool isTranslucent) { m_isTranslucent = isTranslucent; }

            void depthSortTriangles()
            {
                if(m_customVertexIndices.empty())
                {
                    m_customVertexIndices = m_objectData->m_vertexIndices;
                }

                const auto& cameraPos = SingletonManager::get<EngineManager>()->getCamera()->getGlobalPosition();
                const auto& nodePos = getGlobalPosition();
                const auto& vertices = m_objectData->m_vertexData;

                std::sort(
                        m_customVertexIndices.begin(),
                        m_customVertexIndices.end(),
                        [cameraPos, nodePos, vertices](const auto& a, const auto& b)
                        { return depthSortTrianglesAlgorithm(cameraPos, nodePos, vertices, a, b); }
                );

                unsigned int dataSize = m_customVertexIndices.size() * sizeof(triData);
                if(m_customIndexBuffer == 0)
                {
                    // Generate a buffer with our identifier
                    glGenBuffers(1, &m_customIndexBuffer);
                    glBindBuffer(GL_ARRAY_BUFFER, m_customIndexBuffer);

                    // Give vertices to OpenGL
                    glBufferData(GL_ARRAY_BUFFER, dataSize, &m_customVertexIndices[0], GL_STATIC_DRAW);
                }
                else
                {
                    glBindBuffer(GL_ARRAY_BUFFER, m_customIndexBuffer);
                    // Give vertices to OpenGL
                    glBufferData(GL_ARRAY_BUFFER, dataSize, &m_customVertexIndices[0], GL_STATIC_DRAW);
                }
                // glFinish();
            }

            /**
             * @brief Gets the bufferId for Object.
             * @return GLuint
             */
            GLuint getIndexBuffer() const
            {
                if(m_isTranslucent)
                {
                    return m_customIndexBuffer;
                }

                return m_objectData ? m_objectData->m_indexBuffer : 0;
            };

        private:
            std::shared_ptr<ObjectData> m_objectData;
            std::shared_ptr<Shader> m_shader;
            GLuint m_textureBuffer;
            glm::vec4 m_tint;
            bool m_isTranslucent;

            GLuint m_customIndexBuffer;
            std::vector<triData> m_customVertexIndices;
    };

} // namespace Engine
