#pragma once

#include "../../classes/engine/rendering/Shader.h"

class ColorShader : public Engine::Shader
{
    public:
        explicit ColorShader(const std::shared_ptr<Engine::RenderManager>& renderManager);
        ~ColorShader() = default;
};