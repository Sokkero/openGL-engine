
#include "ColorShader.h"

using namespace Engine;

ColorShader::ColorShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/colorShader/color", "color", true, false);

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());
    bindUbo(renderManager->getVpUbo());
}

void ColorShader::renderObject(const std::shared_ptr<GeometryComponent>& object, CameraComponent*)
{
    swapToProgramm();

    loadModelMatrix(object);
    loadTint(object);
    loadVertexBuffer(object);
    loadNormalBuffer(object);
    loadColorBuffer(object);
    drawElements(object);
}
