
#include "Shader.h"
#include "../DebugModel.h"

using namespace Engine;

GLuint Shader::CURRENT_PROGRAMM = -1;

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

void Shader::renderVertices(std::nullptr_t object, Engine::CameraComponent* camera)
{
    loadCustomRenderData(camera);
}

void Shader::renderVertices(const std::shared_ptr<GeometryComponent>& object, Engine::CameraComponent* camera)
{
    const std::shared_ptr<DebugModel>& debugModel = SingletonManager::get<DebugModel>();

    double tempTimestamp = glfwGetTime();

    const std::shared_ptr<ObjectData>& objectData = object->getObjectData();
    const glm::mat4& modelMatrix = object->getGlobalModelMatrix();

    debugModel->setDrawSectionTimeData("1", glfwGetTime() - tempTimestamp);
    tempTimestamp = glfwGetTime();

    swapToProgramm();

    debugModel->setDrawSectionTimeData("2", glfwGetTime() - tempTimestamp);
    tempTimestamp = glfwGetTime();

    glUniformMatrix4fv(getActiveUniform("modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);

    debugModel->setDrawSectionTimeData("3", glfwGetTime() - tempTimestamp);
    tempTimestamp = glfwGetTime();

    const glm::vec4 tint = object->getTint();
    GLuint uniformId = getActiveUniform("tintColor");
    glUniform4f(uniformId, tint.x, tint.y, tint.z, tint.w);

    debugModel->setDrawSectionTimeData("4", glfwGetTime() - tempTimestamp);
    tempTimestamp = glfwGetTime();

    if(objectData->m_vertexBuffer != -1)
    {
        bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXPOSITION, GL_ARRAY_BUFFER, objectData->m_vertexBuffer, 3, GL_FLOAT, false, 0);
    }

    if(objectData->m_normalBuffer != -1)
    {
        bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXNORMAL, GL_ARRAY_BUFFER, objectData->m_normalBuffer, 3, GL_FLOAT, false, 0);
    }

    if(object->getTextureBuffer() != -1)
    {
        if(m_passVisual == PASS_COLOR)
        {
            bindVertexData(GLOBAL_ATTRIB_INDEX_VERTEXCOLOR, GL_ARRAY_BUFFER, object->getTextureBuffer(), 4, GL_FLOAT, false, 0);
        }
        else if(m_passVisual == PASS_TEXTURE)
        {
            bindTexture(
                    GLOBAL_ATTRIB_INDEX_VERTEXCOLOR,
                    objectData->m_uvBuffer,
                    object->getTextureBuffer(),
                    getActiveUniform("textureSampler")
            );
        }
    }

    debugModel->setDrawSectionTimeData("5", glfwGetTime() - tempTimestamp);
    tempTimestamp = glfwGetTime();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->getIndexBuffer());

    debugModel->setDrawSectionTimeData("6", glfwGetTime() - tempTimestamp);
    tempTimestamp = glfwGetTime();

    glDrawElements(
            GL_TRIANGLES,                 // mode
            objectData->getVertexCount(), // count
            GL_UNSIGNED_SHORT,            // type
            nullptr                       // element array buffer offset
    );

    debugModel->setDrawSectionTimeData("7", glfwGetTime() - tempTimestamp);
    tempTimestamp = glfwGetTime();

    RenderUtils::checkForGLError();
    loadCustomRenderData(object, camera);
    RenderUtils::checkForGLError();

    debugModel->setDrawSectionTimeData("8", glfwGetTime() - tempTimestamp);
}

void Shader::swapToProgramm()
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
        fprintf(stderr, "Uniform index not found! Shader invalid");
        return -1;
    }
    else if(index == GL_INVALID_OPERATION)
    {
        fprintf(stderr, "Uniform index not found! Linking failed");
        return -1;
    }

    RenderUtils::checkForGLError();
    return index;
}

void Shader::bindUbo(const std::shared_ptr<UboBlock>& ubo)
{
    if(std::find(m_boundUbos.begin(), m_boundUbos.end(), ubo) != m_boundUbos.end())
    {
        fprintf(stderr, "ubo skip");
        return;
    }

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
    RenderUtils::checkForGLError();
    glBindBuffer(targetType, bufferId);
    RenderUtils::checkForGLError();
    glVertexAttribPointer(attribId, size, dataType, normalized, stride, (void*)nullptr);

    RenderUtils::checkForGLError();
}