#pragma once

#include "../../classes/engine/rendering/Shader.h"

class TextureShader : public Engine::Shader
{
    public:
        explicit TextureShader(const std::shared_ptr<Engine::RenderManager>& renderManager);
        ~TextureShader() = default;

        void renderVertices(std::shared_ptr<Engine::GeometryComponent> object, Engine::CameraComponent* camera) override;
};