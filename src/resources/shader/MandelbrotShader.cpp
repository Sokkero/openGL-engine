
#include "MandelbrotShader.h"
#include "../../customCode/mandelbrotScene/MandelbrotUbo.h"

#include <utility>

using namespace Engine;

MandelbrotShader::MandelbrotShader(const std::shared_ptr<RenderManager>& renderManager, std::shared_ptr<MandelbrotUbo> ubo)
    : m_mandelbrotUbo(std::move(ubo))
{
    registerShader(renderManager, "resources/shader/mandelbrot", "mandelbrot");
}

void MandelbrotShader::renderVertices(GeometryComponent* object, CameraComponent* camera)
{
    if(object->getObjectData()->m_vertexBuffer == -1)
    {
        fprintf(stderr, "Object is missing vertices!");
        return;
    }

    glUseProgram(getShaderIdentifier().second);

    glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() *
            object->getGlobalModelMatrix();

    // Load MVP matrix into uniform
    glUniformMatrix4fv(getActiveUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

    bindVertexData(
            GLOBAL_ATTRIB_INDEX_VERTEXPOSITION,
            GL_ARRAY_BUFFER,
            object->getObjectData()->m_vertexBuffer,
            3,
            GL_FLOAT,
            false,
            0
    );

    // Drawing the object
    glDrawArrays(GL_TRIANGLES, 0, object->getObjectData()->getVertexCount());
    glDisableVertexAttribArray(GLOBAL_ATTRIB_INDEX_VERTEXPOSITION);
}