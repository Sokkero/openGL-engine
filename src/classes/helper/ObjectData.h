#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

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

    static bool depthSortTriangles(const glm::vec3& cameraPos, const glm::vec3& nodePos, const std::vector<glm::vec3>& vertices, const triData& triA, const triData& triB)
    {
        auto aPos = (vertices[std::get<0>(triA)] + vertices[std::get<1>(triA)] + vertices[std::get<2>(triA)]);
        aPos.x = aPos.x / 3;
        aPos.y = aPos.y / 3;
        aPos.z = aPos.z / 3;
        aPos += nodePos;

        auto bPos = (vertices[std::get<0>(triB)] + vertices[std::get<1>(triB)] + vertices[std::get<2>(triB)]);
        bPos.x = bPos.x / 3;
        bPos.y = bPos.y / 3;
        bPos.z = bPos.z / 3;
        bPos += nodePos;

        const auto& distanceA = glm::distance(aPos, cameraPos);
        const auto& distanceB = glm::distance(bPos, cameraPos);

        return distanceA < distanceB;
    }
} // namespace Engine