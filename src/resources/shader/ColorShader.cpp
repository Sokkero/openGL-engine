
#include "ColorShader.h"

using namespace Engine;

ColorShader::ColorShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/color", "color");

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());
    bindUbo(renderManager->getVpUbo());

    setVisualPassStyle(Shader::PASS_COLOR);
}
