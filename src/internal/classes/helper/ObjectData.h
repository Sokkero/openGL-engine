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
                : vertexBuffer(vertexBuffer)
                , uvBuffer(uvBuffer)
                , normalBuffer(normalBuffer)
                , indexBuffer(indexBuffer)
                , vertexData(std::move(vertexData))
                , vertexUvs(std::move(vertexUvs))
                , vertexNormals(std::move(vertexNormals))
                , filePath(std::move(filePath))
                , vertexIndices(std::move(vertexIndices))
            {
                objectId = ID_COUNTER.fetch_add(1);
            }

            bool operator==(const ObjectData& other) const { return objectId == other.objectId; }

            bool operator!=(const ObjectData& other) const { return !(*this == other); }

            std::vector<GLuint> getActiveBuffers()
            {
                std::vector<GLuint> activeBuffers;
                if(vertexBuffer != -1) activeBuffers.emplace_back(vertexBuffer);
                if(uvBuffer != -1) activeBuffers.emplace_back(uvBuffer);
                if(normalBuffer != -1) activeBuffers.emplace_back(normalBuffer);
                if(indexBuffer != -1) activeBuffers.emplace_back(indexBuffer);

                return activeBuffers;
            }

            uint32_t objectId;
            std::string filePath;
            GLuint vertexBuffer;
            GLuint uvBuffer;
            GLuint normalBuffer;
            GLuint indexBuffer;
            std::vector<glm::vec3> vertexData;
            std::vector<glm::vec2> vertexUvs;
            std::vector<glm::vec3> vertexNormals;

            std::vector<triData> vertexIndices;

            int getVertexCount() const { return int(vertexIndices.size() * 3); };

            static std::atomic<uint32_t> ID_COUNTER;
    };
} // namespace Engine