#include "ShaderLoader.h"

#include "classes/utils/RenderUtils.h"
#include "classes/utils/StringFormat.h"

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

std::vector<std::string> ShaderLoader::INCLUDED_FILES;

GLuint ShaderLoader::LoadShader(const char* vertFilePath, const char* fragFilePath)
{
    LOG_DEBUG("ShaderLoader", stringf("Loading shader\nVert: %s\nFrag: %s", vertFilePath, fragFilePath));

    // Read the shader code from their files
    std::string vertShaderCode = loadFile(vertFilePath, {}, true);
    std::string fragShaderCode = loadFile(fragFilePath, {}, true);

    // Compile the shader code
    GLuint vertShaderID = compileShader(vertShaderCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragShaderID = compileShader(fragShaderCode.c_str(), GL_FRAGMENT_SHADER);

    // Link and return programId
    return linkShader(vertShaderID, fragShaderID);
}

std::string ShaderLoader::loadFile(const char* path, std::vector<std::string> args, bool isMainFile /* = false */)
{
    if(isMainFile)
    {
        INCLUDED_FILES.clear();
    }

    std::string glslCode;

    // Load file content
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

    // Files that should never be compiled twice in one shader
    if(glslCode.find("#pragma once") != std::string::npos)
    {
        StringFormat::replaceAll(glslCode, "#pragma once", "");
        INCLUDED_FILES.emplace_back(path);
    }

    // Replace arguments with their passed values
    for(int i = 0; i < args.size(); i++)
    {
        StringFormat::replaceAll(glslCode, stringf("arg[%i]", i), args[i]);
    }

    // Load included files
    std::vector<IncludeDirective> includes = getIncludes(glslCode);
    for(const auto& include : includes)
    {
        if(std::find(INCLUDED_FILES.begin(), INCLUDED_FILES.end(), include.filePath) != INCLUDED_FILES.end())
        {
            LOG_WARN("ShaderLoader", stringf("Duplicate pragma once include will be skipped. \"%s\" includes \"%s\"", path, include.filePath));
            StringFormat::replaceAll(glslCode, include.fullMatch, "");
            continue;
        }

        std::string includedGlslCode = loadFile(include.filePath.c_str(), include.arguments);
        StringFormat::replaceAll(glslCode, include.fullMatch, includedGlslCode);
    }

    return glslCode;
}

std::vector<ShaderLoader::IncludeDirective> ShaderLoader::getIncludes(std::string glslCode)
{
    std::vector<IncludeDirective> includes;

    // Pattern matches: #include <path>(...optional args...)
    // Made flexible to handle both with and without arguments
    std::regex includePattern(R"(#include\s+<"([^"]+)\">\s*(?:\((.*?)\))?)");

    auto begin = std::sregex_iterator(glslCode.begin(), glslCode.end(), includePattern);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;

        IncludeDirective directive;
        directive.fullMatch = match[0].str();          // Full #include statement
        directive.filePath = match[1].str();           // Path between < >

        // Parse arguments if they exist
        if (match[2].matched) {
            directive.arguments = getArguments(match[2].str());
        }

        includes.push_back(directive);
    }

    return includes;
}

std::vector<std::string> ShaderLoader::getArguments(std::string argsString)
{
    std::vector<std::string> args;

    if (argsString.empty()) {
        return args;  // No arguments
    }

    // Match quoted strings: "something"
    std::regex argPattern("\"([^\"]*)\"");

    auto begin = std::sregex_iterator(argsString.begin(), argsString.end(), argPattern);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it)
    {
        args.push_back((*it)[1].str());  // Capture group 1 is content inside quotes
    }

    return args;
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
            ENGINE_ASSERT(false, &shaderMessage[0]);
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
            ENGINE_ASSERT(false, &programMessage[0]);
        }
    }

    glDetachShader(programID, vertId);
    glDetachShader(programID, fragId);

    glDeleteShader(vertId);
    glDeleteShader(fragId);

    Engine::RenderUtils::checkForGLError();

    return programID;
}