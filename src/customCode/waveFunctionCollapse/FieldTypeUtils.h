#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <string>

class Field;

inline static std::vector<glm::ivec2> GetNeighborOffsets()
{
    return {
        glm::ivec2(1.f, 1.f),   glm::ivec2(0.f, 1.f),  glm::ivec2(-1.f, 1.f), glm::ivec2(-1.f, 0.f),
        glm::ivec2(-1.f, -1.f), glm::ivec2(0.f, -1.f), glm::ivec2(1.f, -1.f), glm::ivec2(1.f, 0.f),
    };
}

using grid2d = std::vector<std::vector<std::shared_ptr<Field>>>;
using ruleFunction = std::function<bool(const glm::ivec2& pos, const grid2d& grid)>;

struct BasicFieldDataStruct
{
        std::vector<int> allowedNeighborTileTypeIds;
        glm::vec3 fieldColor;
        int weight;
        int tileTypeId;
        std::vector<ruleFunction> placementRules;

        BasicFieldDataStruct(
                const std::vector<int>& allowedfields,
                const glm::vec3& fieldColor,
                const int weight,
                const int tileTypeId,
                std::vector<ruleFunction> placementRules
        )
            : allowedNeighborTileTypeIds(allowedfields)
            , fieldColor(fieldColor)
            , weight(weight)
            , tileTypeId(tileTypeId)
            , placementRules(placementRules)
        {};

        static bool checkRulesForPosition(const glm::ivec2& pos, const grid2d& grid, BasicFieldDataStruct field)
        {
            for(const ruleFunction& rule : field.placementRules)
            {
                if(!rule(pos, grid))
                {
                    return false;
                }
            }
            return true;
        }

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
