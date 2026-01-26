#pragma once

#include <GL/glew.h>

class ShaderLoader
{
    public:
        ShaderLoader() = delete;

        static GLuint LoadShader(const char* vertex_file_path, const char* fragment_file_path);

    private:
        struct IncludeDirective {
                std::string fullMatch;              // The entire #include statement
                std::string filePath;               // Path inside <>
                std::vector<std::string> arguments; // Parsed arguments
        };

        static std::string loadFile(const char* path, std::vector<std::string> args, bool isMainFile = false);
        static std::vector<IncludeDirective> getIncludes(std::string glslCode);
        static std::vector<std::string> getArguments(std::string argsString);
        static GLuint compileShader(const char* sourcePtr, GLenum shaderType);
        static GLuint linkShader(GLuint vertId, GLuint fragId);

        static std::vector<std::string> INCLUDED_FILES;
};

