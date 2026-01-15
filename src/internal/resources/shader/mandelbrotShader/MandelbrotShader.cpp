#include "MandelbrotShader.h"

#include "exampleScenes/mandelbrotScene/MandelbrotUbo.h"

#include <utility>

using namespace Engine;

MandelbrotShader::MandelbrotShader(const std::shared_ptr<RenderManager>& renderManager, std::shared_ptr<MandelbrotUbo> ubo)
    : m_mandelbrotUbo(std::move(ubo))
{
    registerShader(renderManager, "resources/shader/mandelbrotShader/mandelbrot", "mandelbrot", false, false);

    bindUbo(renderManager->getVpUbo());
}

void MandelbrotShader::renderObject(const std::shared_ptr<GeometryComponent>& object, Engine::CameraComponent* camera)
{
    swapToProgramm();

    loadModelMatrix(object);
    loadVertexBuffer(object);
    drawElements(object);
}
