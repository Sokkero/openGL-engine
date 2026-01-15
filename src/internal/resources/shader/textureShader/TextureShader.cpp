
#include "TextureShader.h"

using namespace Engine;

TextureShader::TextureShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/textureShader/texture", "texture", true, true);

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());
    bindUbo(renderManager->getVpUbo());
}

void TextureShader::renderObject(const std::shared_ptr<GeometryComponent>& object, Engine::CameraComponent* camera)
{
    swapToProgramm();

    loadModelMatrix(object);
    loadTint(object);
    loadVertexBuffer(object->getObjectData());
    loadNormalBuffer(object->getObjectData());
    loadTextureBuffer(object);
    drawElements(object);
}
