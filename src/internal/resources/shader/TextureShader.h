#pragma once

#include "classes/engine/rendering/Shader.h"

namespace Engine
{
    class TextureShader : public Shader
    {
        public:
            explicit TextureShader(const std::shared_ptr<RenderManager>& renderManager);
            ~TextureShader() = default;

            void renderObject(const std::shared_ptr<GeometryComponent>& object, CameraComponent* camera) override;
    };
} // namespace Engine
