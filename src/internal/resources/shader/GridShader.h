#pragma once

#include "classes/engine/rendering/Shader.h"

namespace Engine
{
    class GridShader : public Shader
    {
        public:
            GridShader();
            ~GridShader() = default;

            void renderObject(const std::shared_ptr<GeometryComponent>& object, CameraComponent* camera) override;

            float getGridScale() const { return m_gridScale; };

            void setGridScale(float newScale) { m_gridScale = newScale; };

            float getGridNear() const { return m_gridNear; };

            void setGridNear(float newNear) { m_gridNear = newNear; };

            float getGridFar() const { return m_gridFar; };

            void setGridFar(float newFar) { m_gridFar = newFar; };

        private:
            float m_gridScale;
            float m_gridNear;
            float m_gridFar;
    };
} // namespace Engine