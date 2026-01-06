
#include "AmbientLightUbo.h"

using namespace Engine::UBOs;

AmbientLightUbo::AmbientLightUbo()
    : m_intensity(.2f)
    , m_useAmbient(true)
    , m_color(glm::vec3(1.f))
    , m_data(glm::vec4(1.f))
{
    setSize(16);
    setBindingPoint(AMBIENT_LIGHT_POINT);

    setupUbo();
}

void AmbientLightUbo::updateUbo()
{
    m_data.x = m_color.x;
    m_data.y = m_color.y;
    m_data.z = m_color.z;
    m_data.w = m_useAmbient ? m_intensity : 0.f;

    LoadGlmVariable(m_data, 0);
}

void AmbientLightUbo::setIsActive(bool useAmbient)
{
    m_useAmbient = useAmbient;
    updateUbo();
}

void AmbientLightUbo::setColor(glm::vec3 color)
{
    m_color = glm::vec4(color, 1.f);
    updateUbo();
}

void AmbientLightUbo::setIntensity(float intensity)
{
    m_intensity = intensity;
    updateUbo();
}
