
#include "MandelbrotUbo.h"

MandelbrotUbo::MandelbrotUbo()
    : m_iterations(300)
    , m_zoom(500)
    , m_screenSize(1024, 640)
    , m_offset(1.5, 0.6)
    , m_updated(false)
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

void MandelbrotUbo::checkUpdates()
{
    if(m_updated)
    {
        UpdateUbo();
        m_updated = false;
    }
}

void MandelbrotUbo::setIterations(int itr)
{
    m_iterations = itr;
    m_updated = true;
}

void MandelbrotUbo::setZoom(float zoom)
{
    m_zoom = zoom;
    m_updated = true;
}

void MandelbrotUbo::setScreenSize(glm::vec2 screenSize)
{
    m_screenSize = screenSize;
    m_updated = true;
}

void MandelbrotUbo::setOffset(glm::vec2 offset)
{
    m_offset = offset;
    m_updated = true;
}
