#pragma once

#include "classes/engine/rendering/Shader.h"

namespace Engine
{
    class RenderComponent;

    class ColorShader : public Shader
    {
        public:
            explicit ColorShader(const std::shared_ptr<Engine::RenderManager>& renderManager);
            ~ColorShader() = default;

            void renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent*) override;
    };
} // namespace Engine