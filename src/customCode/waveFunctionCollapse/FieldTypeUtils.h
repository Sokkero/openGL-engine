#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <string>

// Gets all 4 neighbors that are directly adjacent
inline static std::vector<glm::ivec2> GetDirectNeighborOffsets()
{
    return {
        glm::ivec2(0.f, 1.f),
        glm::ivec2(-1.f, 0.f),
        glm::ivec2(0.f, -1.f),
        glm::ivec2(1.f, 0.f),
    };
}

// Gets all 4 neighbors that are touching the corners
inline static std::vector<glm::ivec2> GetCornerNeighborOffsets()
{
    return {
        glm::ivec2(1.f, 1.f),
        glm::ivec2(-1.f, 1.f),
        glm::ivec2(-1.f, -1.f),
        glm::ivec2(1.f, -1.f),
    };
}

// Gets all neighbors in 4 groups for the NE/SE/SW/NW corners
inline static std::vector<std::vector<glm::ivec2>> GetGroupedCornerNeighborOffsets()
{
    return {
        {glm::ivec2(0.f, 1.f), glm::ivec2(1.f, 1.f), glm::ivec2(1.f, 0.f)},
        {glm::ivec2(1.f, 0.f), glm::ivec2(1.f, -1.f), glm::ivec2(0.f, -1.f)},
        {glm::ivec2(0.f, -1.f), glm::ivec2(-1.f, -1.f), glm::ivec2(-1.f, 0.f)},
        {glm::ivec2(-1.f, 0.f), glm::ivec2(-1.f, 1.f), glm::ivec2(0.f, 1.f)},
    };
}

// Gets all neighbors in 4 groups for the N/E/S/W sides
inline static std::vector<std::vector<glm::ivec2>> GetGroupedDirectNeighborOffsets()
{
    return {
        {glm::ivec2(-1.f, 1.f), glm::ivec2(0.f, 1.f), glm::ivec2(1.f, 1.f)},
        {glm::ivec2(1.f, 1.f), glm::ivec2(1.f, 0.f), glm::ivec2(1.f, -1.f)},
        {glm::ivec2(1.f, -1.f), glm::ivec2(0.f, -1.f), glm::ivec2(-1.f, -1.f)},
        {glm::ivec2(-1.f, -1.f), glm::ivec2(-1.f, 0.f), glm::ivec2(-1.f, 1.f)},
    };
}

// Gets all 8 neighbors
inline static std::vector<glm::ivec2> GetNeighborOffsets()
{
    return {
        glm::ivec2(1.f, 1.f),   glm::ivec2(0.f, 1.f),  glm::ivec2(-1.f, 1.f), glm::ivec2(-1.f, 0.f),
        glm::ivec2(-1.f, -1.f), glm::ivec2(0.f, -1.f), glm::ivec2(1.f, -1.f), glm::ivec2(1.f, 0.f),
    };
}

class Field;
using grid2d = std::vector<std::vector<std::shared_ptr<Field>>>;
using ruleFunction = std::function<bool(const glm::ivec2& pos, const grid2d& grid)>;

struct BasicFieldDataStruct
{
        int uniqueTileTypeId;
        std::vector<ruleFunction> placementRules;
        int weight;

        /**
         * @param uniqueTileTypeId The unique id of the tile
         * @param weight The wighting youd like to add to the tile (changes probability of picking this tile)
         * @param placementRules Vector of rules that have to pass in order to place this tile. All rules have to pass.
         */
        BasicFieldDataStruct(const int uniqueTileTypeId, const int weight, const std::vector<ruleFunction>& placementRules)
            : weight(weight)
            , uniqueTileTypeId(uniqueTileTypeId)
            , placementRules(placementRules) {};

        static bool checkRulesForPosition(const glm::ivec2& pos, const grid2d& grid, const BasicFieldDataStruct& field)
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
            return uniqueTileTypeId == other.uniqueTileTypeId;
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
