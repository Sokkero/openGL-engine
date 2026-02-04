#pragma once

#include "classes/engine/rendering/Shader.h"

namespace Engine
{
    class RenderComponent;

    class DebugLineShader : public Shader
    {
        public:
            DebugLineShader();
            ~DebugLineShader() = default;

            void renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent*) override {};
            void renderLines(GLuint linesBuffer, int vertexCount);
    };
} // namespace Engine