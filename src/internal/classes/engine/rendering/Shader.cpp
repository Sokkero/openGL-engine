#include "Shader.h"

#include "classes/engine/DebugModel.h"
#include "classes/utils/RenderUtils.h"

using namespace Engine;

GLuint Shader::CURRENT_PROGRAMM = -1;

Shader::Shader() : m_debugModel(SingletonManager::get<DebugModel>()) {}

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

    RenderUtils::checkForGLError();
}

void Shader::registerShader(
        const std::shared_ptr<RenderManager>& renderManager,
        const std::string& shaderPath,
        const std::string& shaderName,
        bool requiresAdditionalData,
        bool requiresTexture
)
{
    m_shaderIdentifier = renderManager->registerShader(shaderPath, shaderName);
    m_requiresAdditionalData = requiresAdditionalData;
    m_requiresTexture = requiresTexture;
}

void Shader::loadModelMatrix(const glm::mat4& modelMatrix) const
{
    glUniformMatrix4fv(getActiveUniform("modelMatrixUni"), 1, GL_FALSE, &modelMatrix[0][0]);
}

void Shader::loadVertexBuffer(GLuint buffer) const
{
    bindVertexData(RenderUtils::GLOBAL_ATTRIB_ID_VERTEXPOSITION, GL_ARRAY_BUFFER, buffer, 3, GL_FLOAT, false, 0);
}

void Shader::loadUVBuffer(GLuint buffer) const
{
    bindVertexData(RenderUtils::GLOBAL_ATTRIB_ID_VERTEXUV, GL_ARRAY_BUFFER, buffer, 2, GL_FLOAT, false, 0);
}

void Shader::loadNormalBuffer(GLuint buffer) const
{
    bindVertexData(RenderUtils::GLOBAL_ATTRIB_ID_VERTEXNORMAL, GL_ARRAY_BUFFER, buffer, 3, GL_FLOAT, false, 0);
}

void Shader::loadTextureBuffer(GLuint buffer) const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer);
    glUniform1i(getActiveUniform("textureSampler"), 0);

    RenderUtils::checkForGLError();
}

void Shader::drawElements(const std::shared_ptr<RenderComponent>& object) const
{
    glUniform1i(getActiveUniform("isInstanced"), false);

    if(m_requiresAdditionalData)
    {
        object->getShaderData()->loadDataAsUniform(getActiveUniform("additionalDataUni"));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->getIndexBuffer());
    glDrawElements(
            GL_TRIANGLES,                              // mode
            object->getObjectData()->getVertexCount(), // count
            GL_UNSIGNED_SHORT,                         // type
            nullptr                                    // element array buffer offset
    );
    RenderUtils::checkForGLError();
}

void Shader::swapToProgramm() const
{
    if(CURRENT_PROGRAMM != m_shaderIdentifier.second)
    {
        glUseProgram(m_shaderIdentifier.second);
        CURRENT_PROGRAMM = m_shaderIdentifier.second;
        RenderUtils::checkForGLError();
    }
}

GLint Shader::getActiveUniform(const std::string& uniform) const
{
    const GLint index = glGetUniformLocation(m_shaderIdentifier.second, uniform.c_str());

    if(index == GL_INVALID_VALUE)
    {
        ENGINE_ASSERT(false, "Uniform index not found! Shader invalid");
        return -1;
    }
    else if(index == -1)
    {
        ENGINE_ASSERT(false, "Uniform index not found! Linking failed");
        return -1;
    }

    RenderUtils::checkForGLError();
    return index;
}

void Shader::bindUbo(const std::shared_ptr<UboBlock>& ubo)
{
    if(std::find(m_boundUbos.begin(), m_boundUbos.end(), ubo) != m_boundUbos.end())
    {
        return;
    }

    unsigned int index = glGetUniformBlockIndex(m_shaderIdentifier.second, ubo->getBindingPoint().first);
    if(index == GL_INVALID_INDEX)
    {
        ENGINE_ASSERT(false, "Ubo index not found!");
        return;
    }

    glUniformBlockBinding(m_shaderIdentifier.second, index, ubo->getBindingPoint().second);

    m_boundUbos.push_back(ubo);
    RenderUtils::checkForGLError();
}

void Shader::removeBoundUbo(const std::shared_ptr<UboBlock>& ubo)
{
    m_boundUbos.erase(std::remove(m_boundUbos.begin(), m_boundUbos.end(), ubo), m_boundUbos.end());
    RenderUtils::checkForGLError();
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

    RenderUtils::checkForGLError();
}
