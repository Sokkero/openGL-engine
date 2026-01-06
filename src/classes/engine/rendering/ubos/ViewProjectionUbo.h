#include "UboBindingPoints.h"
#include "UboBlock.h"

#include <glm/vec3.hpp>

namespace Engine::UBOs
{
    class ViewProjectionUbo : public UboBlock
    {
        public:
            ViewProjectionUbo();
            ~ViewProjectionUbo() = default;

            void updateUbo() override;
            void updateUbo(glm::mat4 view, glm::mat4 projection);
    };
} // namespace Engine::UBOs