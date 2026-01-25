#include "ShaderLoader.h"

#include "classes/utils/RenderUtils.h"

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>

GLuint ShaderLoader::LoadShader(const char* vertFilePath, const char* fragFilePath)
{
    LOG_DEBUG("ShaderLoader", stringf("Loading shader\nVert: %s\nFrag: %s", vertFilePath, fragFilePath));

    // Read the Vertex Shader code from the file
    std::string vertShaderCode = loadFile(vertFilePath, {}, false);
    std::string fragShaderCode = loadFile(fragFilePath, {}, false);

    GLuint vertShaderID = compileShader(vertShaderCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragShaderID = compileShader(fragShaderCode.c_str(), GL_FRAGMENT_SHADER);

    return linkShader(vertShaderID, fragShaderID);
}

std::string ShaderLoader::loadFile(const char* path, std::vector<std::string> args, bool isUtilFile)
{
    std::string glslCode;

    std::ifstream inputStream(path, std::ios::in);
    if(inputStream.is_open())
    {
        std::stringstream sstr;
        sstr << inputStream.rdbuf();
        glslCode = sstr.str();
        inputStream.close();
    }
    else
    {
        ENGINE_ASSERT(false, stringf("Impossible to open %s", path));
    }

    return glslCode;
}

GLuint ShaderLoader::compileShader(const char* sourcePtr, GLenum shaderType)
{
    GLint result = GL_FALSE;
    int infoLogLength;

    // Compile shader
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &sourcePtr, nullptr);
    glCompileShader(shaderId);

    // Check shader
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
        std::vector<char> shaderMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, &shaderMessage[0]);
        if(result == GL_TRUE)
        {
            LOG_DEBUG("ShaderLoader", &shaderMessage[0]);
        }
        else
        {
            LOG_ERROR("ShaderLoader", &shaderMessage[0]);
        }
    }

    return shaderId;
}

GLuint ShaderLoader::linkShader(GLuint vertId, GLuint fragId)
{
    // Link the program
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertId);
    glAttachShader(programID, fragId);
    glLinkProgram(programID);

    // Check the program
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
        std::vector<char> programMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, nullptr, &programMessage[0]);
        if(result == GL_TRUE)
        {
            LOG_DEBUG("ShaderLoader", &programMessage[0]);
        }
        else
        {
            LOG_ERROR("ShaderLoader", &programMessage[0]);
        }
    }

    glDetachShader(programID, vertId);
    glDetachShader(programID, fragId);

    glDeleteShader(vertId);
    glDeleteShader(fragId);

    Engine::RenderUtils::checkForGLError();

    return programID;
}