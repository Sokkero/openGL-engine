#pragma once

#include "../../classes/engine/rendering/Shader.h"

class SolidColorShader : public Engine::Shader
{
    public:
        explicit SolidColorShader(const std::shared_ptr<Engine::RenderManager>& renderManager);
        ~SolidColorShader() = default;

        void renderVertices(std::shared_ptr<Engine::GeometryComponent> object, Engine::CameraComponent *camera) override;
};