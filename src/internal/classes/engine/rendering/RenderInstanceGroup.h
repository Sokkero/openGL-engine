#pragma once

#include "classes/utils/enums/RenderTypeEnum.h"

#include <unordered_map>
#include <GL/glew.h>
#include <memory>

namespace Engine
{
    class RenderComponent;
    class ObjectData;
    class Shader;
    class RenderManager;
    class AdditionalShaderDataBase;

    class RenderInstanceGroup
    {
        public:
            RenderInstanceGroup(const std::shared_ptr<ObjectData>& objectData, const std::shared_ptr<Shader>& shader, RenderTypeEnum renderType, const std::shared_ptr<AdditionalShaderDataBase>& additionalDataExample = nullptr, GLuint textureId = 0);
            ~RenderInstanceGroup();

            void addToGroup(const std::shared_ptr<RenderComponent>& node);
            void removeFromGroup(uint32_t nodeId); // Use Swap & Pop!!!!

            void refreshAllNodes();
            void refreshDirtyNodes();

            void renderGroup();

            int getActiveCount() const { return m_nodes.size(); }

            bool fitsIntoGroup(std::shared_ptr<RenderComponent>& node);

        private:
            void refreshNode(const std::shared_ptr<RenderComponent>& node);
            void setupVao();
            void growCapacity();

            GLuint m_objectVao;
            GLuint m_matrixVbo;
            GLuint m_dataVbo;

            int m_capacity; // = 32;
            float m_growthFactor; // = 2;

            std::vector<std::shared_ptr<RenderComponent>> m_nodes;
            std::shared_ptr<ObjectData> m_objectData;
            std::shared_ptr<Shader> m_shader;
            std::unordered_map<uint32_t, int> m_nodeIdToIndex;
            RenderTypeEnum m_renderType;

            bool m_requiresAdditionalData;
            size_t m_additionalDataSize;
            GLenum m_additionalDataBaseType;
            GLuint m_additionalDataBaseTypeAmount;

            bool m_requiresTexture;
            GLuint m_textureId;

            std::shared_ptr<RenderManager> m_renderManager;
    };
}
