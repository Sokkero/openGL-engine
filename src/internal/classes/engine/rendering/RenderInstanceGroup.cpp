#include "RenderInstanceGroup.h"

#include "classes/utils/dataContainer/ObjectData.h"
#include "classes/engine/rendering/Shader.h"
#include "classes/utils/RenderUtils.h"
#include "classes/engine/rendering/RenderManager.h"

using namespace Engine;

RenderInstanceGroup::RenderInstanceGroup(const std::shared_ptr<RenderComponent>& node)
    : m_capacity(32)
    , m_growthFactor(2)
    , m_objectData(std::make_shared<ObjectData>(*node->getObjectData()))
    , m_shader(node->getShader())
    , m_renderType(node->getRenderType())
    , m_requiresAdditionalData(node->getShader()->requiresAdditionalData())
    , m_requiresTexture(node->getShader()->requiresTexture())
    , m_renderManager(SingletonManager::get<RenderManager>())
    , m_dataVbo(0)
{
    if(m_renderType == RenderTypeEnum::Loose)
    {
        fprintf(stderr, "Cannot use instanced rendering on type 'loose' objects");
        assert(false);
        return;
    }

    m_nodes.reserve(m_capacity);

    glGenBuffers(1, &m_matrixVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_capacity * sizeof(glm::mat4),
                 nullptr,
                 m_renderType == RenderTypeEnum::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    if(m_requiresAdditionalData)
    {
        m_additionalDataSize = node->getShaderData()->getDataTypeSize();
        m_additionalDataBaseType = node->getShaderData()->getGlDataType();
        m_additionalDataBaseTypeAmount = node->getShaderData()->getGlTypeAmount();

        glGenBuffers(1, &m_dataVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
        glBufferData(GL_ARRAY_BUFFER,
                     m_capacity * m_additionalDataSize,
                     nullptr,
                     m_renderType == RenderTypeEnum::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    if(m_requiresTexture)
    {
        if(node->getTextureBuffer() == 0)
        {
            fprintf(stderr, "Texture required but invalid textureId provided!");
            assert(false);
            return;
        }
        m_textureId = node->getTextureBuffer();
    }

    RenderUtils::checkForGLError();

    setupVao();

    addToGroup(node);
}

RenderInstanceGroup::~RenderInstanceGroup()
{
    glDeleteBuffers(1, &m_matrixVbo);
    if (m_requiresAdditionalData)
    {
        glDeleteBuffers(1, &m_dataVbo);
    }

    glDeleteVertexArrays(1, &m_objectVao);
}

bool RenderInstanceGroup::fitsIntoGroup(const std::shared_ptr<RenderComponent>& node)
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

    // Bind vertex index VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objectData->indexBuffer);

    // Bind default VAO again. All settings are now contained in our VAO
    glBindVertexArray(m_renderManager->getDefaultVao());

    RenderUtils::checkForGLError();
}

bool RenderInstanceGroup::addToGroup(const std::shared_ptr<RenderComponent>& node)
{
    if(!fitsIntoGroup(node))
    {
        return false;
    }

    m_nodes.push_back(node);
    m_nodeIdToIndex[node->getNodeId()] = m_nodes.size() - 1;

    if(m_capacity <= m_nodes.size())
    {
        growCapacity();
        return true;
    }

    refreshNode(node);
    return true;
}

void RenderInstanceGroup::removeFromGroup(uint32_t nodeId)
{
    if(!m_nodeIdToIndex.count(nodeId))
    {
        fprintf(stderr, "Cant remove node which does not exist in this group");
        assert(false);
        return;
    }

    if(m_nodes.size() == 1)
    {
        m_nodes.clear();
        m_nodeIdToIndex.clear();
        return;
    }

    const int nodePos = m_nodeIdToIndex[nodeId];
    if(nodePos == m_nodes.size() - 1)
    {
        m_nodes.pop_back();
        m_nodeIdToIndex.erase(nodeId);
        return;
    }

    m_nodes.at(nodePos) = m_nodes.back();
    m_nodeIdToIndex[m_nodes.at(nodePos)->getNodeId()] = nodePos;
    m_nodes.pop_back();
    m_nodeIdToIndex.erase(nodeId);

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

    if(m_requiresAdditionalData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
        glBufferData(GL_ARRAY_BUFFER,
                     m_capacity * m_additionalDataSize,
                     nullptr,
                     m_renderType == RenderTypeEnum::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

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
        std::vector<char> dataBuffer;
        dataBuffer.reserve(m_nodes.size() * m_additionalDataSize);
        for (const auto& node : m_nodes)
        {
            void* data = node->getShaderData()->getData();
            const char* byteData = static_cast<const char*>(data);
            dataBuffer.insert(dataBuffer.end(), byteData, byteData + m_additionalDataSize);
        }

        // Upload data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        dataBuffer.size(),
                        dataBuffer.data());
    }

    RenderUtils::checkForGLError();
}

void RenderInstanceGroup::refreshDirtyNodes()
{
    for(const auto& node : m_nodes)
    {
        if(node->getIsDirty())
        {
            refreshNode(node);
        }
    }
}

void RenderInstanceGroup::refreshNode(const std::shared_ptr<RenderComponent>& node)
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

    void* data = node->getShaderData()->getData();
    GLuint dataOffset = nodePos * m_additionalDataSize;
    glBindBuffer(GL_ARRAY_BUFFER, m_dataVbo);
    glBufferSubData(GL_ARRAY_BUFFER,
                    dataOffset,
                    m_additionalDataSize,
                    data);

    node->setIsDirty(false);
    RenderUtils::checkForGLError();
}

void RenderInstanceGroup::renderGroup()
{
    if(m_renderType == RenderTypeEnum::Dynamic)
    {
        refreshAllNodes();
    }
    else
    {
        refreshDirtyNodes();
    }

    m_shader->swapToProgramm();

    glUniform1i(m_shader->getActiveUniform("isInstanced"), true);

    if(m_requiresTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glUniform1i(m_shader->getActiveUniform("textureSampler"), 0);
    }

    glBindVertexArray(m_objectVao);
    glDrawElementsInstanced(GL_TRIANGLES,
                            m_objectData->getVertexCount(),
                            GL_UNSIGNED_SHORT,
                            0,
                            m_nodes.size());
    glBindVertexArray(m_renderManager->getDefaultVao());
}
