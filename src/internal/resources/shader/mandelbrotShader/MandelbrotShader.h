#pragma once

#include "classes/engine/rendering/Shader.h"

class MandelbrotUbo;

namespace Engine
{
    class RenderComponent;

    class MandelbrotShader : public Shader
    {
        public:
            MandelbrotShader(const std::shared_ptr<RenderManager>& renderManager, std::shared_ptr<MandelbrotUbo> ubo);
            ~MandelbrotShader() = default;

            void renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent* camera) override;

        private:
            std::shared_ptr<MandelbrotUbo> m_mandelbrotUbo;
    };
} // namespace Engine
