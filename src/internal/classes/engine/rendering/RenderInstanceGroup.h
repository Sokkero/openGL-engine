#pragma once

#include "utils/enums/RenderTypeEnum.h"

#include <GL/glew.h>
#include <memory>
#include <unordered_map>

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
            explicit RenderInstanceGroup(const std::shared_ptr<RenderComponent>& node);
            ~RenderInstanceGroup();

            bool addToGroup(const std::shared_ptr<RenderComponent>& node);
            void removeFromGroup(uint32_t nodeId); // Use Swap & Pop!!!!

            void refreshAllNodes();
            void refreshDirtyNodes();

            void renderGroup();

            int getActiveCount() const { return m_nodes.size(); }

        private:
            bool fitsIntoGroup(const std::shared_ptr<RenderComponent>& node);
            void refreshNode(const std::shared_ptr<RenderComponent>& node);
            void setupVao();
            void growCapacity();

            GLuint m_objectVao;
            GLuint m_matrixVbo;
            GLuint m_dataVbo;

            int m_capacity;       // = 32;
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
} // namespace Engine
