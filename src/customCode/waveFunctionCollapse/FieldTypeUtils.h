#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <string>

inline static std::vector<glm::ivec2> GetNeighborOffsets()
{
    return {
        glm::ivec2(1.f, 1.f),   glm::ivec2(0.f, 1.f),  glm::ivec2(-1.f, 1.f), glm::ivec2(-1.f, 0.f),
        glm::ivec2(-1.f, -1.f), glm::ivec2(0.f, -1.f), glm::ivec2(1.f, -1.f), glm::ivec2(1.f, 0.f),
    };
}

struct BasicFieldDataStruct
{
        std::vector<int> allowedNeighborTileTypeIds;
        glm::vec3 fieldColor;
        int weight;
        int tileTypeId;

        BasicFieldDataStruct(
                const std::vector<int>& allowedfields,
                const glm::vec3& fieldColor,
                const int weight,
                const int tileTypeId
        )
            : allowedNeighborTileTypeIds(allowedfields)
            , fieldColor(fieldColor)
            , weight(weight)
            , tileTypeId(tileTypeId) {};

        bool operator==(const BasicFieldDataStruct& other) const
        {
            return allowedNeighborTileTypeIds == other.allowedNeighborTileTypeIds &&
                    tileTypeId == other.tileTypeId;
        }
};

inline static void AddFieldWeighting(std::vector<BasicFieldDataStruct>& fields)
{
    std::vector<BasicFieldDataStruct> fieldsToAdd;
    for(const auto& field : fields)
    {
        for(int i = 1; i < field.weight; ++i)
        {
            fieldsToAdd.push_back(field);
        }
    }

    for(const auto& field : fieldsToAdd)
    {
        fields.push_back(field);
    }
}
