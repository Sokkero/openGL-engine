#pragma once

#include "classes/nodeComponents/BasicNode.h"
#include "utils/dataContainer/AdditionalShaderDataBase.h"
#include "utils/dataContainer/TriDataDef.h"
#include "utils/enums/RenderTypeEnum.h"

#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <memory>
#include <utility>

namespace Engine
{
    class Shader;
    class ObjectData;

    class RenderComponent
        : virtual public BasicNode
        , public std::enable_shared_from_this<RenderComponent>
    {
        public:
            RenderComponent();
            ~RenderComponent() = default;

            std::shared_ptr<ObjectData> getObjectData() const { return m_objectData; };
            void setObjectData(const std::shared_ptr<ObjectData>& objData);

            GLuint getTextureBuffer() const { return m_textureBuffer; };
            void setTextureBuffer(GLuint buffer);

            std::shared_ptr<Shader> getShader() const { return m_shader; };
            void setShader(const std::shared_ptr<Shader>& shader);

            bool getIsTranslucent() const { return m_isTranslucent; };
            void setIsTranslucent(bool isTranslucent) { m_isTranslucent = isTranslucent; }

            RenderTypeEnum getRenderType() const { return m_renderType; };
            void setRenderType(RenderTypeEnum renderType);

            AdditionalShaderDataBase* getShaderData() { return m_additionalShaderData.get(); };
            void setShaderData(std::unique_ptr<AdditionalShaderDataBase> shaderData);

            void depthSortTriangles();

            GLuint getIndexBuffer() const;

            void setIsActiveInScene(bool active) { m_activeInScene = active; }

        private:
            void resortInRenderManager();

            bool m_activeInScene;

            std::shared_ptr<ObjectData> m_objectData;
            std::shared_ptr<Shader> m_shader;
            std::unique_ptr<AdditionalShaderDataBase> m_additionalShaderData;
            RenderTypeEnum m_renderType;

            GLuint m_textureBuffer;
            bool m_isTranslucent;

            GLuint m_depthSortIndexBuffer;
            std::vector<triData> m_depthSortedVertexIndices;
    };

} // namespace Engine
