#pragma once

#include "../../rendering/Shader.h"

namespace Engine
{
    class GridShader : public Shader
    {
        public:
            GridShader(const std::shared_ptr<RenderManager>& renderManager);
            ~GridShader() = default;

            void renderVertices(GeometryComponent* object, CameraComponent* camera) override;
    };
}