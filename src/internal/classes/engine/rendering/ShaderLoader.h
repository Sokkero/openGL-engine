#pragma once

#include <GL/glew.h>

class ShaderLoader
{
    public:
        ShaderLoader() = default;
        ~ShaderLoader() = default;

        static GLuint LoadShader(const char* vertex_file_path, const char* fragment_file_path);

    private:
        static std::string loadFile(const char* path, std::vector<std::string> args, bool isUtilFile);
        static GLuint compileShader(const char* sourcePtr, GLenum shaderType);
        static GLuint linkShader(GLuint vertId, GLuint fragId);
};

