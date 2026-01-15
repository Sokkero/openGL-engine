#include "RenderInstanceGroup.h"

#include "classes/utils/dataContainer/ObjectData.h"
#include "classes/engine/rendering/Shader.h"
#include "classes/utils/RenderUtils.h"
#include "classes/engine/rendering/RenderManager.h"

using namespace Engine;

RenderInstanceGroup::RenderInstanceGroup(const std::shared_ptr<ObjectData>& objectData, const std::shared_ptr<Shader>& shader, RenderTypeEnum renderType, const std::shared_ptr<AdditionalShaderDataBase>& additionalDataExample, GLuint textureId)
    : m_capacity(32)
    , m_growthFactor(2)
    , m_activeCount(0)
    , m_objectData(objectData)
    , m_shader(shader)
    , m_renderType(renderType)
    , m_requiresAdditionalData(shader->requiresAdditionalData())
    , m_requiresTexture(shader->requiresTexture())
    , m_renderManager(SingletonManager::get<RenderManager>())
{
    if(m_renderType == RenderTypeEnum::Loose)
    {
        fprintf(stderr, "Cannot use instanced rendering on type 'loose' objects");
        assert(false);
        return;
    }

    m_nodes.resize(m_capacity);

    glGenBuffers(1, &m_matrixVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_capacity * sizeof(glm::mat4),
                 nullptr,
                 m_renderType == RenderTypeEnum::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    if(m_requiresAdditionalData)
    {
        if(!additionalDataExample)
        {
            fprintf(stderr, "Requiering example of addional data!");
            assert(false);
            return;
        }

        m_additionalDataSize = additionalDataExample->getDataTypeSize();
        m_additionalDataBaseType = additionalDataExample->getGlDataType();
        m_additionalDataBaseTypeAmount = additionalDataExample->getGlTypeAmount();

        glGenBuffers(1, &m_dataVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
        glBufferData(GL_ARRAY_BUFFER,
                     m_capacity * m_additionalDataSize,
                     nullptr,
                     m_renderType == RenderTypeEnum::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    if(m_requiresTexture)
    {
        m_textureId = textureId;
    }

    RenderUtils::checkForGLError();

    setupVao();
}

bool RenderInstanceGroup::fitsIntoGroup(std::shared_ptr<GeometryComponent>& node)
{
    if(node->getObjectData()->objectId != m_objectData->objectId)
    {
        return false;
    }
    else if(node->getShader()->getShaderIdentifier() != m_shader->getShaderIdentifier())
    {
        return false;
    }
    else if(m_requiresTexture && m_textureId != node->getTextureBuffer())
    {
        return false;
    }

    return true;
}

void RenderInstanceGroup::setupVao()
{
    // Create and bind VAO, VAOs "contain" binding data
    glGenVertexArrays(1, &m_objectVao);
    glBindVertexArray(m_objectVao);

    // Bind mesh specific data
    m_shader->loadVertexBuffer(m_objectData);
    m_shader->loadNormalBuffer(m_objectData);
    m_shader->loadUVBuffer(m_objectData);

    // Create & bind matrix VBO, setup with instancing dividers
    // Instance matrices (locations 3-6, mat4 = 4x vec4)
    glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo); // Bind matrix VBO
    GLuint attribArrayId = RenderUtils::GLOBAL_ATTRIB_ID_MATRICES;
    for(int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(attribArrayId + i); // Enable attribArray x
        glVertexAttribPointer(
                attribArrayId + i,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glm::mat4),
                (void*)(sizeof(glm::vec4) * i)
        ); // Define data
        glVertexAttribDivisor(
                attribArrayId + i,
                1
        ); // Setup divisor logic (0 = all for all instances, 1 = 1 per instance, 2 = 2 per instance)
    }

    if (m_requiresAdditionalData)
    {
        // Create & bind data VBO, setup with instancing dividers
        attribArrayId = RenderUtils::GLOBAL_ATTRIB_ID_ADDITIONAL_SHADER_DATA;
        glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
        glEnableVertexAttribArray(attribArrayId);
        glVertexAttribPointer(attribArrayId,
                              m_additionalDataBaseTypeAmount,
                              m_additionalDataBaseType,
                              GL_FALSE,
                              m_additionalDataSize,
                              0);
        glVertexAttribDivisor(attribArrayId, 1);
    }

    if(m_requiresTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glUniform1i(m_shader->getActiveUniform("textureSampler"), 0);
    }

    // Bind vertex index VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objectData->indexBuffer);

    // Bind default VAO again. All settings are now contained in our VAO
    glBindVertexArray(m_renderManager->getDefaultVao());

    RenderUtils::checkForGLError();
}

void RenderInstanceGroup::addToGroup(const std::shared_ptr<GeometryComponent>& node)
{
    m_nodes.at(m_activeCount) = node;
    m_nodeIdToIndex[node->getNodeId()] = m_activeCount;

    m_activeCount++;
    if(m_capacity < m_activeCount)
    {
        growCapacity();
        return;
    }

    refreshNode(node);
}

void RenderInstanceGroup::removeFromGroup(uint32_t nodeId)
{
    if(!m_nodeIdToIndex.count(nodeId))
    {
        fprintf(stderr, "Cant remove node which does not exist in this group");
        assert(false);
        return;
    }

    const int nodePos = m_nodeIdToIndex[nodeId];
    m_nodes.at(nodePos) = m_nodes.at(m_activeCount - 1);
    m_nodes.at(m_activeCount - 1) = nullptr;

    m_nodeIdToIndex[m_nodes.at(nodePos)->getNodeId()] = nodePos;
    m_nodeIdToIndex.erase(nodeId);

    m_activeCount--;

    refreshNode(m_nodes.at(nodePos));
}

void RenderInstanceGroup::growCapacity()
{
    m_capacity = m_capacity * m_growthFactor;

    glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_capacity * sizeof(glm::mat4),
                 nullptr,
                 m_renderType == RenderTypeEnum::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_capacity * m_additionalDataSize,
                 nullptr,
                 m_renderType == RenderTypeEnum::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    RenderUtils::checkForGLError();

    refreshAllNodes();
}

void RenderInstanceGroup::refreshAllNodes()
{
    // Collect matrices from all nodes
    std::vector<glm::mat4> matrices;
    matrices.reserve(m_nodes.size());
    for (const auto& node : m_nodes)
    {
        matrices.push_back(node->getGlobalModelMatrix());
    }

    // Upload matrices to GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    matrices.size() * sizeof(glm::mat4),
                    matrices.data());

    if (m_requiresAdditionalData)
    {
        std::vector<void*> dataVec;
        dataVec.reserve(m_nodes.size());
        for (const auto& node : m_nodes)
        {
            void* data = node->getShaderData()->getData();
            dataVec.push_back(data);
        }

        // Upload data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        dataVec.size() * m_additionalDataSize,
                        dataVec.data());
    }

    RenderUtils::checkForGLError();
}

void RenderInstanceGroup::refreshDirtyNodes()
{
    for(const auto& node : m_nodes)
    {
        if(node->getIsRenderDataDirty())
        {
            refreshNode(node);
        }
    }
}

void RenderInstanceGroup::refreshNode(const std::shared_ptr<GeometryComponent>& node)
{
    int nodePos = m_nodeIdToIndex[node->getNodeId()];

    glm::mat4 matrix = node->getGlobalModelMatrix();
    GLuint matrixOffset = nodePos * sizeof(glm::mat4);
    glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
    glBufferSubData(GL_ARRAY_BUFFER,
                    matrixOffset,
                    sizeof(glm::mat4),
                    &matrix);

    RenderUtils::checkForGLError();

    if(!m_requiresAdditionalData)
    {
        return;
    }

    void* data = node->getShaderData();
    GLuint dataOffset = nodePos * m_additionalDataSize;
    glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
    glBufferSubData(GL_ARRAY_BUFFER,
                    dataOffset,
                    m_additionalDataSize,
                    data);

    RenderUtils::checkForGLError();
}

void RenderInstanceGroup::renderGroup()
{
    m_shader->swapToProgramm();

    glBindVertexArray(m_objectVao);
    glDrawElementsInstanced(GL_TRIANGLES,
                            m_objectData->getVertexCount(),
                            GL_UNSIGNED_INT,
                            0,
                            m_nodes.size());
}
