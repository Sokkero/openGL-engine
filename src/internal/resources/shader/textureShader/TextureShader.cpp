
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

    loadModelMatrix(object->getGlobalModelMatrix());
    loadVertexBuffer(object->getObjectData()->vertexBuffer);
    loadUVBuffer(object->getObjectData()->uvBuffer);
    loadNormalBuffer(object->getObjectData()->normalBuffer);
    loadTextureBuffer(object->getTextureBuffer());

    drawElements(object);
}
