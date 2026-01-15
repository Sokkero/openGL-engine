#pragma once

#include "classes/nodeComponents/BasicNode.h"
#include "classes/utils/dataContainer/TriDataDef.h"
#include "classes/utils/enums/RenderTypeEnum.h"
#include "classes/utils/dataContainer/AdditionalShaderDataBase.h"

#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <memory>
#include <utility>

namespace Engine
{
    class Shader;
    class ObjectData;

    /**
     * @brief The GeometryComponent class represents a component that handles geometry-related operations for a node.
     */
    class GeometryComponent : virtual public BasicNode
    {
        public:
            GeometryComponent();
            ~GeometryComponent() = default;

            glm::vec4 getTint() const { return m_tint; };
            void setTint(glm::vec4 tint);

            std::shared_ptr<ObjectData> getObjectData() const { return m_objectData; };
            void setObjectData(std::shared_ptr<ObjectData> objData) { m_objectData = std::move(objData); };

            GLuint getTextureBuffer() const { return m_textureBuffer; };
            void setTextureBuffer(GLuint buffer) { m_textureBuffer = buffer; };

            std::shared_ptr<Shader> getShader() const { return m_shader; };
            void setShader(std::shared_ptr<Shader> shader) { m_shader = std::move(shader); }

            bool getIsTranslucent() const { return m_isTranslucent; };
            void setIsTranslucent(bool isTranslucent) { m_isTranslucent = isTranslucent; }

            bool getIsRenderDataDirty() const { return m_isRenderDataDirty; };
            void setIsRenderDataDirty(bool dirty) { m_isRenderDataDirty = dirty; }

            RenderTypeEnum getRenderType() const { return m_renderType; };
            void setRenderType(RenderTypeEnum renderType) { m_renderType = renderType; }

            AdditionalShaderDataBase* getShaderData() { return m_additionalShaderData.get(); };
            void setShaderData(std::unique_ptr<AdditionalShaderDataBase> shaderData);

            void depthSortTriangles();

            GLuint getIndexBuffer() const;

        private:
            std::shared_ptr<ObjectData> m_objectData;
            std::shared_ptr<Shader> m_shader;
            std::unique_ptr<AdditionalShaderDataBase> m_additionalShaderData;
            RenderTypeEnum m_renderType;
            bool m_isRenderDataDirty;

            GLuint m_textureBuffer;
            glm::vec4 m_tint;
            bool m_isTranslucent;

            GLuint m_depthSortIndexBuffer;
            std::vector<triData> m_depthSortedVertexIndices;
    };

} // namespace Engine
