#include "../../classes/engine/rendering/UboBlock.h"
#include "../../classes/engine/rendering/lighting/LightingPoints.h"

#include <glm/vec2.hpp>

class MandelbrotUbo : public Engine::UboBlock
{
    public:
        MandelbrotUbo();
        ~MandelbrotUbo() = default;

        void UpdateUbo() override;

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