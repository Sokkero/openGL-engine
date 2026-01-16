
#include "TextureShader.h"

using namespace Engine;

TextureShader::TextureShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/textureShader/texture", "texture", false, true);

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());
    bindUbo(renderManager->getVpUbo());
}

void TextureShader::renderObject(const std::shared_ptr<RenderComponent>& object, Engine::CameraComponent* camera)
{
    swapToProgramm();

    loadModelMatrix(object);
    loadVertexBuffer(object->getObjectData());
    loadNormalBuffer(object->getObjectData());
    loadTextureBuffer(object);

    drawElements(object);
}
