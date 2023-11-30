
#include "DiffuseLightUbo.h"

using namespace Engine::Lighting;

DiffuseLightUbo::DiffuseLightUbo() : m_intensity(1.f), m_useDiffuse(true), m_color(glm::vec3(1.f)), m_direction(glm::vec3(1.f))
{
    setSize(48);
    setBindingPoint(DIFFUSE_LIGHT_POINT);

    setupUbo();
}

void DiffuseLightUbo::UpdateUbo()
{
    LoadVariable(m_useDiffuse, 0);
    LoadVariable(m_intensity, 4);
    LoadVariable(m_direction, 16);
    LoadVariable(m_color, 32);
}
