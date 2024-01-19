
#include "TextureShader.h"

using namespace Engine;

TextureShader::TextureShader(const std::shared_ptr<RenderManager>& renderManager)
{
    registerShader(renderManager, "resources/shader/texture", "texture");

    bindUbo(renderManager->getAmbientLightUbo());
    bindUbo(renderManager->getDiffuseLightUbo());
}

void TextureShader::renderVertices(std::shared_ptr<GeometryComponent> object, CameraComponent* camera)
{
    if(object->getObjectData()->m_vertexBuffer == -1)
    {
        fprintf(stderr, "Object is missing vertices!");
        return;
    }

    if(object->getObjectData()->m_normalBuffer == -1)
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
            object->getObjectData()->m_vertexBuffer,
            3,
            GL_FLOAT,
            false,
            0
    );
    bindVertexData(
            GLOBAL_ATTRIB_INDEX_VERTEXNORMAL,
            GL_ARRAY_BUFFER,
            object->getObjectData()->m_normalBuffer,
            3,
            GL_FLOAT,
            false,
            0
    );

    bindTexture(
            GLOBAL_ATTRIB_INDEX_VERTEXCOLOR,
            GL_ARRAY_BUFFER,
            object->getObjectData()->m_uvBuffer,
            2,
            GL_FLOAT,
            false,
            0,
            GL_TEXTURE0,
            GL_TEXTURE_2D,
            object->getTextureBuffer(),
            getActiveUniform("textureSampler")
    );

    // Drawing the object
    glDrawArrays(GL_TRIANGLES, 0, object->getObjectData()->getVertexCount());
    glDisableVertexAttribArray(GLOBAL_ATTRIB_INDEX_VERTEXPOSITION);
    glDisableVertexAttribArray(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR);
    glDisableVertexAttribArray(GLOBAL_ATTRIB_INDEX_VERTEXNORMAL);
}