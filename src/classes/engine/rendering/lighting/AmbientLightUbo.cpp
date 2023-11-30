
#include "AmbientLightUbo.h"

using namespace Engine::Lighting;

AmbientLightUbo::AmbientLightUbo() : m_intensity(.5f), m_useAmbient(true), m_color(glm::vec3(1.f, 1.f, 1.f))
{
    setSize(32);
    setBindingPoint(AMBIENT_LIGHT_POINT);

    setupUbo();
}

void AmbientLightUbo::UpdateUbo()
{
    LoadVariable(m_useAmbient, 0);
    LoadVariable(m_intensity, 4);
    LoadVariable(m_color, 16);
}
