#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "TriDataDef.h"

namespace Engine
{
    struct ObjectData
    {
            ObjectData(
                    std::string filePath,
                    GLuint vertexBuffer,
                    GLuint uvBuffer,
                    GLuint normalBuffer,
                    GLuint indexBuffer,
                    std::vector<glm::vec3> vertexData,
                    std::vector<glm::vec2> vertexUvs,
                    std::vector<glm::vec3> vertexNormals,
                    std::vector<triData> vertexIndices
            )
                : m_vertexBuffer(vertexBuffer)
                , m_uvBuffer(uvBuffer)
                , m_normalBuffer(normalBuffer)
                , m_indexBuffer(indexBuffer)
                , m_vertexData(std::move(vertexData))
                , m_vertexUvs(std::move(vertexUvs))
                , m_vertexNormals(std::move(vertexNormals))
                , m_filePath(std::move(filePath))
                , m_vertexIndices(std::move(vertexIndices))
            {
            }

            std::string m_filePath;
            GLuint m_vertexBuffer;
            GLuint m_uvBuffer;
            GLuint m_normalBuffer;
            GLuint m_indexBuffer;
            std::vector<glm::vec3> m_vertexData;
            std::vector<glm::vec2> m_vertexUvs;
            std::vector<glm::vec3> m_vertexNormals;

            std::vector<triData> m_vertexIndices;

            int getVertexCount() const { return int(m_vertexIndices.size() * 3); };
    };
} // namespace Engine