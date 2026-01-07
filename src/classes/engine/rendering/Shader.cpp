
#include "Shader.h"
#include "../DebugModel.h"

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
        const std::string& shaderName
)
{
    m_shaderIdentifier = renderManager->registerShader(shaderPath, shaderName);
}

void Shader::loadModelMatrix(const std::shared_ptr<GeometryComponent>& object) const
{
    double startTime = glfwGetTime();

    const glm::mat4& modelMatrix = object->getGlobalModelMatrix();
    glUniformMatrix4fv(getActiveUniform("modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);

    m_debugModel->setDrawSectionTimeData("loadModelMatrix", glfwGetTime() - startTime);
}

void Shader::loadTint(const std::shared_ptr<GeometryComponent>& object) const
{
    double startTime = glfwGetTime();

    const glm::vec4 tint = object->getTint();
    GLuint uniformId = getActiveUniform("tintColor");
    glUniform4f((GLint)uniformId, tint.x, tint.y, tint.z, tint.w);

    m_debugModel->setDrawSectionTimeData("loadTint", glfwGetTime() - startTime);
}

void Shader::loadVertexBuffer(const std::shared_ptr<GeometryComponent>& object) const
{
    double startTime = glfwGetTime();

    bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXPOSITION, GL_ARRAY_BUFFER, object->getObjectData()->m_vertexBuffer, 3, GL_FLOAT, false, 0);

    m_debugModel->setDrawSectionTimeData("loadVertexBuffer", glfwGetTime() - startTime);
}

void Shader::loadNormalBuffer(const std::shared_ptr<GeometryComponent>& object) const
{
    double startTime = glfwGetTime();

    bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXNORMAL, GL_ARRAY_BUFFER, object->getObjectData()->m_normalBuffer, 3, GL_FLOAT, false, 0);

    m_debugModel->setDrawSectionTimeData("loadNormalBuffer", glfwGetTime() - startTime);
}

void Shader::loadTextureBuffer(const std::shared_ptr<GeometryComponent>& object) const
{
    double startTime = glfwGetTime();

    bindTexture(
            GLOBAL_ATTRIB_INDEX_VERTEXCOLOR,
            object->getObjectData()->m_uvBuffer,
            object->getTextureBuffer(),
            getActiveUniform("textureSampler")
    );

    m_debugModel->setDrawSectionTimeData("loadTextureBuffer", glfwGetTime() - startTime);
}

void Shader::loadColorBuffer(const std::shared_ptr<GeometryComponent>& object) const
{
    double startTime = glfwGetTime();

    bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR, GL_ARRAY_BUFFER, object->getTextureBuffer(), 4, GL_FLOAT, false, 0);

    m_debugModel->setDrawSectionTimeData("loadColorBuffer", glfwGetTime() - startTime);
}

void Shader::drawElements(const std::shared_ptr<GeometryComponent>& object) const
{
    double startTime = glfwGetTime();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->getIndexBuffer());
    glDrawElements(
            GL_TRIANGLES,                               // mode
            object->getObjectData()->getVertexCount(),  // count
            GL_UNSIGNED_SHORT,                          // type
            nullptr                                     // element array buffer offset
    );

    m_debugModel->setDrawSectionTimeData("drawElements", glfwGetTime() - startTime);
}


void Shader::swapToProgramm() const
{
    if(CURRENT_PROGRAMM != m_shaderIdentifier.second)
    {
        glUseProgram(m_shaderIdentifier.second);
        CURRENT_PROGRAMM = m_shaderIdentifier.second;
    }
}

GLint Shader::getActiveUniform(const std::string& uniform) const
{
    const GLint index = glGetUniformLocation(m_shaderIdentifier.second, uniform.c_str());

    if(index == GL_INVALID_VALUE)
    {
        fprintf(stderr, "Uniform index not found! Shader invalid\n");
        assert(false);
        return -1;
    }
    else if(index == GL_INVALID_OPERATION)
    {
        fprintf(stderr, "Uniform index not found! Linking failed\n");
        assert(false);
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
        fprintf(stderr, "Ubo index not found!\n");
        assert(false);
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
    RenderUtils::checkForGLError();
    glBindBuffer(targetType, bufferId);
    RenderUtils::checkForGLError();
    glVertexAttribPointer(attribId, size, dataType, normalized, stride, (void*)nullptr);

    RenderUtils::checkForGLError();
}