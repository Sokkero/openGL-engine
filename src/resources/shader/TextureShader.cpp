
#include "TextureShader.h"

using namespace Engine;

TextureShader::TextureShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/texture", "texture");

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());

    setVisualPassStyle(Shader::PASS_TEXTURE);
}
