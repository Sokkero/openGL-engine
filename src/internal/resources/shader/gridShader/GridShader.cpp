
#include "GridShader.h"

#include "classes/engine/DebugModel.h"

using namespace Engine;

GridShader::GridShader() : m_gridScale(1.f), m_gridNear(0.01f), m_gridFar(20.f)
{
    std::shared_ptr<RenderManager> renderManager = SingletonManager::get<RenderManager>();

    registerShader(renderManager, "resources/shader/gridShader/grid", "grid", false, false);
    bindUbo(renderManager->getVpUbo());
}

void GridShader::renderObject(const std::shared_ptr<RenderComponent>& object, CameraComponent* camera)
{
    swapToProgramm();

    double startTime = glfwGetTime();
    glUniform1f(getActiveUniform("mainGridScale"), m_gridScale);
    glUniform1f(getActiveUniform("secondaryGridScale"), m_gridScale * 0.1f);

    glUniform1f(getActiveUniform("near"), m_gridNear);
    glUniform1f(getActiveUniform("far"), m_gridFar);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_debugModel->setDrawSectionTimeData("processGridData", glfwGetTime() - startTime);
}