#pragma once

#include <glm/glm.hpp>
#include <map>

#include "TriDataDef.h"

namespace Engine::VertexIndexingUtils
{
    static inline void writeToTuple(triData& tuple, const unsigned short& valueIndex, const unsigned short& value)
    {
        switch(valueIndex)
        {
            case 0:
                std::get<0>(tuple) = value;
                break;
            case 1:
                std::get<1>(tuple) = value;
                break;
            case 2:
                std::get<2>(tuple) = value;
                break;
            default:
                fprintf(stderr, "Vertex indexing failed!");
        }
    }

    struct PackedVertex
    {
            glm::vec3 position;
            glm::vec2 uv;
            glm::vec3 normal;

            bool operator<(const PackedVertex that) const
            {
                return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
            };
    };

    bool getSimilarVertexIndex_fast(
            PackedVertex& packed,
            std::map<PackedVertex, unsigned short>& VertexToOutIndex,
            unsigned short& result
    )
    {
        const auto it = VertexToOutIndex.find(packed);
        if(it == VertexToOutIndex.end())
        {
            return false;
        }
        else
        {
            result = it->second;
            return true;
        }
    }

    void indexVBO(
            std::vector<glm::vec3>& in_vertices,
            std::vector<glm::vec2>& in_uvs,
            std::vector<glm::vec3>& in_normals,
            std::vector<triData>& in_indices
    )
    {
        std::map<PackedVertex, unsigned short> VertexToOutIndex;
        std::vector<triData> out_indices(in_vertices.size() / 3);
        std::vector<glm::vec3> out_vertices;
        std::vector<glm::vec2> out_uvs;
        std::vector<glm::vec3> out_normals;

        // For each input vertex
        for(unsigned int i = 0; i < in_vertices.size(); i++)
        {
            const unsigned short tupleIndex = i / 3;
            const unsigned short valueIndex = i % 3;

            PackedVertex packed = { in_vertices[i],
                                    (i < in_uvs.size() ? in_uvs[i] : glm::vec2(0.f)),
                                    (i < in_normals.size() ? in_normals[i] : glm::vec3(0.f)) };

            // Try to find a similar vertex in out_XXXX
            unsigned short index;
            bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

            if(found)
            { // A similar vertex is already in the VBO, use it instead !
                writeToTuple(out_indices[tupleIndex], valueIndex, index);
            }
            else
            { // If not, it needs to be added in the output data.
                out_vertices.push_back(in_vertices[i]);
                out_uvs.push_back(i < in_uvs.size() ? in_uvs[i] : glm::vec2(0.f));
                out_normals.push_back(i < in_normals.size() ? in_normals[i] : glm::vec3(0.f));
                unsigned short newindex = (unsigned short)out_vertices.size() - 1;
                writeToTuple(out_indices[tupleIndex], valueIndex, newindex);
                VertexToOutIndex[packed] = newindex;
            }
        }

        in_vertices.swap(out_vertices);
        in_uvs.swap(out_uvs);
        in_normals.swap(out_normals);
        in_indices.swap(out_indices);
    }
}
