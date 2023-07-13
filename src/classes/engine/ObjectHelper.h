#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

namespace Engine
{
    enum ShaderType
    {
        solidColor, solidTexture, undefined
    };

    struct ObjectData
    {
        ObjectData(
                std::string filePath,
                GLuint vertexBuffer,
                std::vector<glm::vec3> vertexData,
                std::vector<glm::vec3> vertexNormals
        )
                : m_vertexBuffer(vertexBuffer)
                , m_vertexData(std::move(vertexData))
                , m_vertexNormals(std::move(vertexNormals))
                , m_filePath(filePath)
        {
        }

        std::string m_filePath;
        GLuint m_vertexBuffer;
        std::vector<glm::vec3> m_vertexData;
        std::vector<glm::vec3> m_vertexNormals;

        int getVertexCount() const { return int(m_vertexData.size()); };
    };

    static std::string ShaderTypeToString(ShaderType type)
    {
        switch (type) {
            case ShaderType::solidColor:
                return "solidColor";
            case ShaderType::solidTexture:
                return "solidTexture";
            case ShaderType::undefined:
                return "undefined";
        }
    }
}