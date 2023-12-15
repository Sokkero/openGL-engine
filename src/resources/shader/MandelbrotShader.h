#pragma once

#include "../../classes/engine/rendering/Shader.h"

class MandelbrotUbo;

class MandelbrotShader : public Engine::Shader
{
    public:
        explicit MandelbrotShader(
                const std::shared_ptr<Engine::RenderManager>& renderManager,
                std::shared_ptr<MandelbrotUbo> ubo
        );
        ~MandelbrotShader() = default;

        void renderVertices(Engine::GeometryComponent *object, Engine::CameraComponent *camera) override;

    private:
        std::shared_ptr<MandelbrotUbo> m_mandelbrotUbo;
};