#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Engine
{
    enum class ShaderType
    {
        solidColor,
        solidTexture,
        undefined
    };

    struct ObjectData
    {
            ObjectData(
                    std::string filePath,
                    GLuint vertexBuffer,
                    GLuint uvBuffer,
                    GLuint normalBuffer,
                    std::vector<glm::vec3> vertexData,
                    std::vector<glm::vec2> vertexUvs,
                    std::vector<glm::vec3> vertexNormals
            )
                : m_vertexBuffer(vertexBuffer)
                , m_uvBuffer(uvBuffer)
                , m_normalBuffer(normalBuffer)
                , m_vertexData(std::move(vertexData))
                , m_vertexUvs(std::move(vertexUvs))
                , m_vertexNormals(std::move(vertexNormals))
                , m_filePath(std::move(filePath))
            {
            }

            std::string m_filePath;
            GLuint m_vertexBuffer;
            GLuint m_uvBuffer;
            GLuint m_normalBuffer;
            std::vector<glm::vec3> m_vertexData;
            std::vector<glm::vec2> m_vertexUvs;
            std::vector<glm::vec3> m_vertexNormals;

            int getVertexCount() const { return int(m_vertexData.size()); };
    };

    inline std::string ShaderTypeToString(ShaderType type)
    {
        switch(type)
        {
            case ShaderType::solidColor:
                return "solidColor";
            case ShaderType::solidTexture:
                return "solidTexture";
            case ShaderType::undefined:
                return "undefined";
        }
    }
} // namespace Engine