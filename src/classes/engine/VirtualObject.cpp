
#include "VirtualObject.h"

#include "RenderManager.h"

namespace Engine
{
    VirtualObject::VirtualObject(std::shared_ptr<ObjectData>& objectData)
    : m_objectData(objectData)
    , m_shader(ShaderType::undefined)
    , m_textureBuffer(0)
    , m_tint(glm::vec4(0.f, 0.f, 0.f, 0.f))
    , m_matrixId(0)
    {
    }

    void VirtualObject::setShader(ShaderType shader, RenderManager* renderManager)
    {
        m_shader = shader;
        m_matrixId = renderManager->getUniform(shader, "MVP");
    }
}