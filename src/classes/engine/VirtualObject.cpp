
#include "VirtualObject.h"

namespace Engine
{
    VirtualObject::VirtualObject(std::shared_ptr<ObjectData>& objectData)
    : m_objectData(objectData)
    , m_modelMatrix(glm::mat4(1.f))
    , m_shader(ShaderType::undefined)
    , m_textureBuffer(0)
    , m_tint(glm::vec4(0.f, 0.f, 0.f, 0.f))
    , m_matrixId(0)
    {
    }

    void VirtualObject::setPosition(glm::vec3 pos)
    {
        m_modelMatrix[3] = glm::vec4(pos, 1.f);
    }

    void VirtualObject::moveObj(glm::vec3 dirVec)
    {
        m_modelMatrix = glm::translate(m_modelMatrix, dirVec);
    }

    void VirtualObject::rotateObj(glm::vec3 dirVec, float degrees)
    {
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(degrees), dirVec);
    }

    void VirtualObject::setShader(ShaderType shader, RenderManager& renderManager)
    {
        m_shader = shader;
        m_matrixId = renderManager.getUniform(shader, "MVP");
    }
}