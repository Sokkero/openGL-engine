#include "RenderComponent.h"

#include "classes/engine/EngineManager.h"
#include "classes/nodeComponents/CameraComponent.h"
#include "classes/utils/dataContainer/AdditionalShaderDataBase.h"
#include "classes/utils/dataContainer/ObjectData.h"

using namespace Engine;

RenderComponent::RenderComponent()
    : m_objectData(nullptr)
    , m_shader(nullptr)
    , m_textureBuffer(0)
    , m_tint(glm::vec4(1.f, 1.f, 1.f, 1.f))
    , m_isTranslucent(false)
    , m_depthSortIndexBuffer(0)
    , m_depthSortedVertexIndices(std::vector<triData>())
    , m_additionalShaderData(nullptr)
    , m_renderType(RenderTypeEnum::Static)
    , m_isRenderDataDirty(false)
{
    setIsTranslucent(m_tint.w < 1.f);
}

void RenderComponent::setTint(glm::vec4 tint)
{
    m_tint = tint;
    setIsTranslucent(m_tint.w < 1.f);
};

void RenderComponent::setShaderData(std::unique_ptr<AdditionalShaderDataBase> shaderData)
{
    m_additionalShaderData = std::move(shaderData);
}

void RenderComponent::depthSortTriangles()
{
    if(m_depthSortedVertexIndices.empty())
    {
        m_depthSortedVertexIndices = m_objectData->vertexIndices;
    }

    const auto& cameraPos = SingletonManager::get<EngineManager>()->getCamera()->getGlobalPosition();
    const auto& nodePos = getGlobalPosition();
    const auto& vertices = m_objectData->vertexData;

    std::sort(
            m_depthSortedVertexIndices.begin(),
            m_depthSortedVertexIndices.end(),
            [cameraPos, nodePos, vertices](const auto& a, const auto& b)
            { return depthSortTrianglesAlgorithm(cameraPos, nodePos, vertices, a, b); }
    );

    unsigned int dataSize = m_depthSortedVertexIndices.size() * sizeof(triData);
    if(m_depthSortIndexBuffer == 0)
    {
        // Generate a buffer with our identifier
        glGenBuffers(1, &m_depthSortIndexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_depthSortIndexBuffer);

        // Give vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, dataSize, &m_depthSortedVertexIndices[0], GL_STATIC_DRAW);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_depthSortIndexBuffer);
        // Give vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, dataSize, &m_depthSortedVertexIndices[0], GL_STATIC_DRAW);
    }
    // glFinish();
}

/**
             * @brief Gets the bufferId for Object.
             * @return GLuint
 */
GLuint RenderComponent::getIndexBuffer() const
{
    if(m_isTranslucent)
    {
        return m_depthSortIndexBuffer;
    }

    return m_objectData ? m_objectData->indexBuffer : 0;
};
