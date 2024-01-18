#pragma once

#include "../../rendering/Shader.h"

namespace Engine
{
    class GridShader : public Shader
    {
        public:
            GridShader(const std::shared_ptr<RenderManager>& renderManager);
            ~GridShader() = default;

            void renderVertices(std::shared_ptr<GeometryComponent> object, CameraComponent* camera) override;
    };
} // namespace Engine