
#include "GridShader.h"

using namespace Engine;

GridShader::GridShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/grid", "grid");
}

void GridShader::renderVertices(std::shared_ptr<GeometryComponent> object, CameraComponent* camera)
{
    glUseProgram(getShaderIdentifier().second);

    glUniformMatrix4fv(getActiveUniform("projection"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);
    glUniformMatrix4fv(getActiveUniform("view"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}