#pragma once

#include "../../classes/engine/rendering/Shader.h"

class TextureShader : public Engine::Shader
{
    public:
        explicit TextureShader(const std::shared_ptr<Engine::RenderManager>& renderManager);
        ~TextureShader() = default;
};