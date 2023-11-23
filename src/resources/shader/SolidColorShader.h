#pragma once

#include "../../classes/engine/rendering/Shader.h"

class SolidColorShader : Engine::Shader
{
    public:
        explicit SolidColorShader(Engine::RenderManager& renderManager);
        ~SolidColorShader() = default;

        void renderVertices(Engine::GeometryComponent* object, const glm::mat4& mvp) override;

        glm::vec4 getColor() { return color; }

        void setColor(glm::vec4 newColor) { color = newColor; }

    private:
        glm::vec4 color;
};