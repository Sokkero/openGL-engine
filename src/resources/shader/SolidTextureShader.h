#pragma once

#include "../../classes/engine/rendering/Shader.h"

class SolidTextureShader : public Engine::Shader
{
    public:
        explicit SolidTextureShader(const std::shared_ptr<Engine::RenderManager>& renderManager);
        ~SolidTextureShader() = default;

        void renderVertices(std::shared_ptr<Engine::GeometryComponent> object, Engine::CameraComponent* camera) override;
};