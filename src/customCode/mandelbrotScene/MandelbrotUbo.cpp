
#include "MandelbrotUbo.h"

MandelbrotUbo::MandelbrotUbo() : m_iterations(300), m_zoom(400), m_screenSize(1200, 600), m_offset(0, 0)
{
    setSize(24);
    setBindingPoint({ "MandelbrotBlock", 5 });

    setupUbo();
}

void MandelbrotUbo::UpdateUbo()
{
    LoadVariable(m_iterations, 0);
    LoadVariable(m_zoom, 4);
    LoadVariable(m_screenSize, 8);
    LoadVariable(m_offset, 16);
}

void MandelbrotUbo::resetData()
{
    m_iterations = 300;
    m_zoom = 400;
    m_offset.x = 0;
    m_offset.y = 0;

    UpdateUbo();
}

void MandelbrotUbo::setIterations(int itr)
{
    m_iterations = itr;
    LoadVariable(m_iterations, 0);
}

void MandelbrotUbo::setZoom(float zoom)
{
    m_zoom = zoom;
    LoadVariable(m_zoom, 4);
}

void MandelbrotUbo::setScreenSize(glm::vec2 screenSize)
{
    m_screenSize = screenSize;
    LoadVariable(m_screenSize, 8);
}

void MandelbrotUbo::setOffset(glm::vec2 offset)
{
    m_offset = offset;
    LoadVariable(m_offset, 16);
}
