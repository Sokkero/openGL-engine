
#include "ViewProjectionUbo.h"

using namespace Engine::UBOs;

ViewProjectionUbo::ViewProjectionUbo()
{
    setSize(128);
    setBindingPoint(VP_POINT);

    setupUbo();
}

void ViewProjectionUbo::updateUbo() { updateUbo(glm::mat4(0.f), glm::mat4(0.f)); }

void ViewProjectionUbo::updateUbo(glm::mat4 view, glm::mat4 projection)
{
    LoadGlmVariable(view, 0);
    LoadGlmVariable(projection, 64);
}
