
#include "GridShader.h"

#include "classes/engine/DebugModel.h"

using namespace Engine;

GridShader::GridShader()
{
    std::shared_ptr<RenderManager> renderManager = SingletonManager::get<RenderManager>();

    registerShader(renderManager, "resources/shader/gridShader/grid", "grid", false, false);
    bindUbo(renderManager->getVpUbo());
}

void GridShader::renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent* camera)
{
    swapToProgramm();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}