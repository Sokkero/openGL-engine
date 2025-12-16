#include "UboBindingPoints.h"
#include "UboBlock.h"

#include <glm/vec3.hpp>

namespace Engine::UBOs
{
    class DiffuseLightUbo : public UboBlock
    {
        public:
            DiffuseLightUbo();
            ~DiffuseLightUbo() = default;

            void updateUbo() override;

            bool isActive() const { return m_useDiffuse; };

            void setIsActive(bool useDiffuse);

            glm::vec3 getDir() const { return m_direction; };

            void setDir(glm::vec3 dir);

            glm::vec3 getColor() const { return m_color; };

            void setColor(glm::vec3 color);

            float getIntensity() const { return m_intensity; };

            void setIntensity(float intensity);

        private:
            bool m_useDiffuse;
            glm::vec3 m_color;
            float m_intensity;

            glm::vec4 m_direction;
            glm::vec4 m_colorIntensity;
    };
} // namespace Engine::UBOs