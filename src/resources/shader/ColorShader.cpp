
#include "ColorShader.h"

using namespace Engine;

ColorShader::ColorShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/color", "color");

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());
}

void ColorShader::renderVertices(std::shared_ptr<GeometryComponent> object, CameraComponent* camera)
{
    const auto& objectData = object->getObjectData();

    if(objectData->m_vertexBuffer == -1)
    {
        fprintf(stderr, "Object is missing vertices!");
        return;
    }

    if(objectData->m_normalBuffer == -1)
    {
        fprintf(stderr, "Object is missing vertex normals!");
        return;
    }

    if(object->getTextureBuffer() == -1)
    {
        fprintf(stderr, "Object is missing vertex normals!");
        return;
    }

    glUseProgram(getShaderIdentifier().second);

    glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * object->getGlobalModelMatrix();

    // Load MVP matrix into uniform
    glUniformMatrix4fv(getActiveUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

    // Load tint value into uniform
    const glm::vec4 tint = object->getTint();
    glUniform4f(getActiveUniform("tintColor"), tint.x, tint.y, tint.z, tint.w);

    bindVertexData(
            GLOBAL_ATTRIB_INDEX_VERTEXPOSITION,
            GL_ARRAY_BUFFER,
            objectData->m_vertexBuffer,
            3,
            GL_FLOAT,
            false,
            0
    );
    bindVertexData(
            GLOBAL_ATTRIB_INDEX_VERTEXNORMAL,
            GL_ARRAY_BUFFER,
            objectData->m_normalBuffer,
            3,
            GL_FLOAT,
            false,
            0
    );
    bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR, GL_ARRAY_BUFFER, object->getTextureBuffer(), 4, GL_FLOAT, false, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectData->m_indexBuffer);

    // Drawing the object
    glDrawElements(
            GL_TRIANGLES,      // mode
            objectData->getVertexCount(),    // count
            GL_UNSIGNED_SHORT, // type
            nullptr           // element array buffer offset
    );

    glDisableVertexAttribArray(GLOBAL_ATTRIB_INDEX_VERTEXPOSITION);
    glDisableVertexAttribArray(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR);
    glDisableVertexAttribArray(GLOBAL_ATTRIB_INDEX_VERTEXNORMAL);
}