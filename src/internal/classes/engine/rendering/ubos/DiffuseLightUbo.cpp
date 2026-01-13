
#include "DiffuseLightUbo.h"

using namespace Engine::UBOs;

DiffuseLightUbo::DiffuseLightUbo()
    : m_intensity(1.f)
    , m_useDiffuse(true)
    , m_color(glm::vec3(1.f))
    , m_direction(glm::vec4(1.f))
    , m_colorIntensity(glm::vec4(1.f))
{
    setSize(32);
    setBindingPoint(DIFFUSE_LIGHT_POINT);

    setupUbo();
}

void DiffuseLightUbo::updateUbo()
{
    m_colorIntensity.x = m_color.x;
    m_colorIntensity.y = m_color.y;
    m_colorIntensity.z = m_color.z;
    m_colorIntensity.w = m_useDiffuse ? m_intensity : 0.f;

    LoadGlmVariable(m_colorIntensity, 0);
    LoadGlmVariable(m_direction, 16);
}

void DiffuseLightUbo::setDir(glm::vec3 dir)
{
    m_direction = glm::vec4(dir, 0.f);
    updateUbo();
}

void DiffuseLightUbo::setColor(glm::vec3 color)
{
    m_color = color;
    updateUbo();
}

void DiffuseLightUbo::setIntensity(float intensity)
{
    m_intensity = intensity;
    updateUbo();
}

void DiffuseLightUbo::setIsActive(bool useDiffuse)
{
    m_useDiffuse = useDiffuse;
    updateUbo();
}
