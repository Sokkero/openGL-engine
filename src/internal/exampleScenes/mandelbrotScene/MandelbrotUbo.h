#pragma once

#include "classes/engine/rendering/ubos/UboBindingPoints.h"
#include "classes/engine/rendering/ubos/UboBlock.h"

#include <glm/vec2.hpp>

using namespace Engine;

class MandelbrotUbo : public UboBlock
{
    public:
        MandelbrotUbo();
        ~MandelbrotUbo() = default;

        void updateUbo() override;

        int getIterations() const { return m_iterations; };

        void setIterations(int itr);

        float getZoom() const { return m_zoom; };

        void setZoom(float zoom);

        glm::vec2 getScreenSize() const { return m_screenSize; };

        void setScreenSize(glm::vec2 screenSize);

        glm::vec2 getOffset() const { return m_offset; };

        void setOffset(glm::vec2 offset);

        void resetData();

    private:
        int m_iterations;
        float m_zoom;
        glm::vec2 m_screenSize;
        glm::vec2 m_offset;
};