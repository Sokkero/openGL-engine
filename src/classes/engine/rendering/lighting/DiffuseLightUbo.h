#include "../UboBlock.h"
#include "LightingPoints.h"

#include <glm/vec3.hpp>

namespace Engine::Lighting
{
    class DiffuseLightUbo : public UboBlock
    {
        public:
            DiffuseLightUbo();
            ~DiffuseLightUbo() = default;

            void UpdateUbo() override;

            bool isActive() const { return m_useDiffuse; };

            void setIsActive(bool useDiffuse) { m_useDiffuse = useDiffuse; };

            glm::vec3 getDir() const { return m_direction; };

            void setDir(glm::vec3 dir) { m_direction = dir; };

            glm::vec3 getColor() const { return m_color; };

            void setColor(glm::vec3 color) { m_color = color; };

            float getIntensity() const { return m_intensity; };

            void setIntensity(float intensity) { m_intensity = intensity; };

        private:
            int m_useDiffuse;
            glm::vec3 m_direction;
            glm::vec3 m_color;
            float m_intensity;
    };
} // namespace Engine::Lighting