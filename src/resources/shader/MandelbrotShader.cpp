
#include "MandelbrotShader.h"
#include "../../customCode/mandelbrotScene/MandelbrotUbo.h"

#include <utility>

using namespace Engine;

MandelbrotShader::MandelbrotShader(const std::shared_ptr<RenderManager>& renderManager, std::shared_ptr<MandelbrotUbo> ubo)
    : m_mandelbrotUbo(std::move(ubo))
{
    registerShader(renderManager, "resources/shader/mandelbrot", "mandelbrot");
}
