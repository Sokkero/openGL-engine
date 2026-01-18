
#include "ColorShader.h"

using namespace Engine;

ColorShader::ColorShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/colorShader/color", "color", true, false);

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());
    bindUbo(renderManager->getVpUbo());
}

void ColorShader::renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent*)
{
    swapToProgramm();

    loadModelMatrix(object->getGlobalModelMatrix());
    loadVertexBuffer(object->getObjectData()->vertexBuffer);
    loadNormalBuffer(object->getObjectData()->normalBuffer);
    drawElements(object);
}
