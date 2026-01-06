
#include "GridShader.h"

using namespace Engine;

GridShader::GridShader(const std::shared_ptr<RenderManager>& renderManager)
    : m_gridScale(1.f)
    , m_gridNear(0.01f)
    , m_gridFar(20.f)
{
    registerShader(renderManager, "resources/shader/grid", "grid");
}

void GridShader::renderObject(const std::shared_ptr<GeometryComponent>& object, CameraComponent* camera)
{
    swapToProgramm();

    glUniform1f(getActiveUniform("mainGridScale"), m_gridScale);
    glUniform1f(getActiveUniform("secondaryGridScale"), m_gridScale * 0.1f);

    glUniform1f(getActiveUniform("near"), m_gridNear);
    glUniform1f(getActiveUniform("far"), m_gridFar);

    glUniformMatrix4fv(getActiveUniform("projection"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);
    glUniformMatrix4fv(getActiveUniform("view"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}