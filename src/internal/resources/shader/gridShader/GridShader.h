#pragma once

#include "classes/engine/rendering/Shader.h"

namespace Engine
{
    class GridShader : public Shader
    {
        public:
            GridShader();
            ~GridShader() = default;

            void renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent* camera) override;
    };
} // namespace Engine