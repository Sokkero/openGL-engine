#include "../LightingPoints.h"
#include "../UboBlock.h"

#include <glm/vec3.hpp>

namespace Engine::Lighting
{
    class AmbientLightUbo : public UboBlock
    {
        public:
            AmbientLightUbo();
            ~AmbientLightUbo() = default;

            void UpdateUbo() override;

            bool isActive() const { return m_useAmbient; };

            void setIsActive(bool useAmbient);

            glm::vec3 getColor() const { return m_color; };

            void setColor(glm::vec3 color);

            float getIntensity() const { return m_intensity; };

            void setIntensity(float intensity);

        private:
            int m_useAmbient;
            float m_intensity;
            glm::vec3 m_color;
    };
} // namespace Engine::Lighting