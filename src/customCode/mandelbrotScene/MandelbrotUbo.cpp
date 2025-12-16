
#include "MandelbrotUbo.h"

MandelbrotUbo::MandelbrotUbo() : m_iterations(300), m_zoom(400), m_screenSize(glm::vec2(1200, 600)), m_offset(glm::vec2(0, 0))
{
    setSize(24);
    setBindingPoint({ "MandelbrotBlock", 5 });

    setupUbo();
}

void MandelbrotUbo::updateUbo()
{
    LoadGlmVariable(m_screenSize, 0);
    LoadGlmVariable(m_offset, 8);
    LoadBasicVariable(m_iterations, 16);
    LoadBasicVariable(m_zoom, 20);
}

void MandelbrotUbo::resetData()
{
    m_iterations = 300;
    m_zoom = 400;
    m_offset.x = 0;
    m_offset.y = 0;

    updateUbo();
}

void MandelbrotUbo::setScreenSize(glm::vec2 screenSize)
{
    m_screenSize = screenSize;
    LoadGlmVariable(m_screenSize, 0);
}

void MandelbrotUbo::setOffset(glm::vec2 offset)
{
    m_offset = offset;
    LoadGlmVariable(m_offset, 8);
}

void MandelbrotUbo::setIterations(int itr)
{
    m_iterations = itr;
    LoadBasicVariable(m_iterations, 16);
}

void MandelbrotUbo::setZoom(float zoom)
{
    m_zoom = zoom;
    LoadBasicVariable(m_zoom, 20);
}
