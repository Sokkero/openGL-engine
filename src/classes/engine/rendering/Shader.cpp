
#include "Shader.h"

using namespace Engine;

Shader::Shader() : m_passVisual(PASS_NONE) {}

Shader::~Shader()
{
    // TODO: check if this is the correct way to handle expired programms
    GLint numShaders;
    glGetProgramiv(m_shaderIdentifier.second, GL_ATTACHED_SHADERS, &numShaders);

    // Create an array to store the shader object IDs
    auto* shaderIds = new GLuint[numShaders];

    // Get the attached shader primitives
    glGetAttachedShaders(m_shaderIdentifier.second, numShaders, nullptr, shaderIds);

    // Detach and delete the shader primitives if needed
    for(int i = 0; i < numShaders; ++i)
    {
        GLuint shaderId = shaderIds[i];
        glDetachShader(m_shaderIdentifier.second, shaderId);
        glDeleteShader(shaderId);
    }

    // Finally, delete the program
    glDeleteProgram(m_shaderIdentifier.second);
    delete[](shaderIds);
}

void Shader::registerShader(
        const std::shared_ptr<RenderManager>& renderManager,
        const std::string& shaderPath,
        const std::string& shaderName
)
{
    m_shaderIdentifier = renderManager->registerShader(shaderPath, shaderName);
}

void Shader::renderVertices(std::nullptr_t object, Engine::CameraComponent* camera)
{
    loadCustomRenderData(camera);
}

void Shader::renderVertices(const std::shared_ptr<GeometryComponent>& object, Engine::CameraComponent* camera)
{
    const auto& objectData = object->getObjectData();
    glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * object->getGlobalModelMatrix();

    // #PIF This needs to be done once and sent to the shaders, instead of per object
    glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix();

    // #PIF This needs to be sent to the shader and then calculated there
    // #PIF Also, the globalModelMatrix should be cached and not need to be calculated each frame
    mvp = mvp * object->getGlobalModelMatrix();
    // #PIF Cache currently bound shader and only switch if required
    glUseProgram(getShaderIdentifier().second);

    glUniformMatrix4fv(getActiveUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

    const glm::vec4 tint = object->getTint();
    glUniform4f(getActiveUniform("tintColor"), tint.x, tint.y, tint.z, tint.w);

    if(objectData->m_vertexBuffer != -1)
    {
        bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXPOSITION, GL_ARRAY_BUFFER, objectData->m_vertexBuffer, 3, GL_FLOAT, false, 0);
        m_usedAttribArrays.push_back(GLOBAL_ATTRIB_INDEX_VERTEXPOSITION);
    }

    if(objectData->m_normalBuffer != -1)
    {
        bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXNORMAL, GL_ARRAY_BUFFER, objectData->m_normalBuffer, 3, GL_FLOAT, false, 0);
        m_usedAttribArrays.push_back(GLOBAL_ATTRIB_INDEX_VERTEXNORMAL);
    }

    if(object->getTextureBuffer() != -1)
    {
        if(m_passVisual == PASS_COLOR)
        {
            bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR, GL_ARRAY_BUFFER, object->getTextureBuffer(), 4, GL_FLOAT, false, 0);
            m_usedAttribArrays.push_back(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR);
        }
        else if(m_passVisual == PASS_TEXTURE)
        {
            bindTexture(
                    GLOBAL_ATTRIB_INDEX_VERTEXCOLOR,
                    objectData->m_uvBuffer,
                    object->getTextureBuffer(),
                    getActiveUniform("textureSampler")
            );
            m_usedAttribArrays.push_back(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR);
        }
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->getIndexBuffer());

    glDrawElements(
            GL_TRIANGLES,                 // mode
            objectData->getVertexCount(), // count
            GL_UNSIGNED_SHORT,            // type
            nullptr                       // element array buffer offset
    );

    loadCustomRenderData(object, camera);

    // #PIF Remove this entirely
    for(const GLuint arrayIndex : m_usedAttribArrays)
    {
        glDisableVertexAttribArray(arrayIndex);
    }
    m_usedAttribArrays.clear();
}

GLint Shader::getActiveUniform(const std::string& uniform) const
{
    const GLint index = glGetUniformLocation(m_shaderIdentifier.second, uniform.c_str());

    if(index == GL_INVALID_VALUE)
    {
        fprintf(stderr, "Uniform index not found! Shader invalid");
        return -1;
    }
    else if(index == GL_INVALID_OPERATION)
    {
        fprintf(stderr, "Uniform index not found! Linking failed");
        return -1;
    }

    return index;
}

void Shader::bindUbo(const std::shared_ptr<UboBlock>& ubo)
{
    unsigned int index = glGetUniformBlockIndex(m_shaderIdentifier.second, ubo->getBindingPoint().first);

    if(index == GL_INVALID_INDEX)
    {
        fprintf(stderr, "Ubo index not found!");
        return;
    }

    glUniformBlockBinding(m_shaderIdentifier.second, index, ubo->getBindingPoint().second);

    m_boundUbos.push_back(ubo);
}

void Shader::removeBoundUbo(const std::shared_ptr<UboBlock>& ubo)
{
    m_boundUbos.erase(std::remove(m_boundUbos.begin(), m_boundUbos.end(), ubo), m_boundUbos.end());
}

void Shader::bindTexture(GLuint attribId, GLuint bufferId, GLuint textureBufferId, GLint textureSamplerUniformId)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBufferId);
    glUniform1i(textureSamplerUniformId, 0);

    bindVertexData(attribId, GL_ARRAY_BUFFER, bufferId, 2, GL_FLOAT, false, 0);
}

void Shader::bindVertexData(
        GLuint attribId,
        GLenum targetType,
        GLuint bufferId,
        int size,
        GLenum dataType,
        bool normalized,
        int stride
)
{
    glEnableVertexAttribArray(attribId);
    glBindBuffer(targetType, bufferId);
    glVertexAttribPointer(attribId, size, dataType, normalized, stride, (void*)nullptr);
}