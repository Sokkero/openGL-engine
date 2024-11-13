
#include "CustomFieldTypeData.h"

DeepWaterFieldDataStruct::DeepWaterFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::deepWater,
              1,
              { [&](const glm::ivec2& pos, const grid2d& grid) -> bool
                {
                    // All neighboring tiles have to be deepWater or shallowWater
                    for(glm::ivec2 offset : GetNeighborOffsets())
                    {
                        offset += pos;
                        if(offset.x < 0 || offset.x >= grid.size() || offset.y < 0 ||
                           offset.y >= grid.front().size())
                        {
                            continue; // Out of bounds
                        }

                        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                                grid[offset.x][offset.y]->getAllPossibleFieldTypes();

                        const bool canBeShallowWater = std::find(
                                                               possibleFieldTypesOfNeighborField.begin(),
                                                               possibleFieldTypesOfNeighborField.end(),
                                                               ShallowWaterFieldDataStruct()
                                                       ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeDeepWater = std::find(
                                                            possibleFieldTypesOfNeighborField.begin(),
                                                            possibleFieldTypesOfNeighborField.end(),
                                                            DeepWaterFieldDataStruct()
                                                    ) != possibleFieldTypesOfNeighborField.end();

                        if(!canBeDeepWater && !canBeShallowWater)
                        {
                            return false;
                        }
                    }

                    return true;
                } }
      )
{
}

ShallowWaterFieldDataStruct::ShallowWaterFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::shallowWater,
              2,
              { [&](const glm::ivec2& pos, const grid2d& grid) -> bool
                {
                    // All neighboring tiles have to be deepWater or shallowWater or beach
                    for(glm::ivec2 offset : GetNeighborOffsets())
                    {
                        offset += pos;
                        if(offset.x < 0 || offset.x >= grid.size() || offset.y < 0 ||
                           offset.y >= grid.front().size())
                        {
                            continue; // Out of bounds
                        }

                        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                                grid[offset.x][offset.y]->getAllPossibleFieldTypes();

                        const bool canBeBeach = std::find(
                                                        possibleFieldTypesOfNeighborField.begin(),
                                                        possibleFieldTypesOfNeighborField.end(),
                                                        BeachFieldDataStruct()
                                                ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeShallowWater = std::find(
                                                               possibleFieldTypesOfNeighborField.begin(),
                                                               possibleFieldTypesOfNeighborField.end(),
                                                               ShallowWaterFieldDataStruct()
                                                       ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeDeepWater = std::find(
                                                            possibleFieldTypesOfNeighborField.begin(),
                                                            possibleFieldTypesOfNeighborField.end(),
                                                            DeepWaterFieldDataStruct()
                                                    ) != possibleFieldTypesOfNeighborField.end();

                        if(!canBeDeepWater && !canBeShallowWater && !canBeBeach)
                        {
                            return false;
                        }
                    }

                    return true;
                } }
      )
{
}

BeachFieldDataStruct::BeachFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::beach,
              3,
              { [&](const glm::ivec2& pos, const grid2d& grid) -> bool
                {
                    // All neighboring tiles have to be shallowWater or beach or gras
                    for(glm::ivec2 offset : GetNeighborOffsets())
                    {
                        offset += pos;
                        if(offset.x < 0 || offset.x >= grid.size() || offset.y < 0 ||
                           offset.y >= grid.front().size())
                        {
                            continue; // Out of bounds
                        }

                        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                                grid[offset.x][offset.y]->getAllPossibleFieldTypes();

                        const bool canBeBeach = std::find(
                                                        possibleFieldTypesOfNeighborField.begin(),
                                                        possibleFieldTypesOfNeighborField.end(),
                                                        BeachFieldDataStruct()
                                                ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeShallowWater = std::find(
                                                               possibleFieldTypesOfNeighborField.begin(),
                                                               possibleFieldTypesOfNeighborField.end(),
                                                               ShallowWaterFieldDataStruct()
                                                       ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeGras = std::find(
                                                       possibleFieldTypesOfNeighborField.begin(),
                                                       possibleFieldTypesOfNeighborField.end(),
                                                       GrasFieldDataStruct()
                                               ) != possibleFieldTypesOfNeighborField.end();

                        if(!canBeGras && !canBeShallowWater && !canBeBeach)
                        {
                            return false;
                        }
                    }

                    return true;
                } }
      )
{
}

GrasFieldDataStruct::GrasFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::grass,
              5,
              { [&](const glm::ivec2& pos, const grid2d& grid) -> bool
                {
                    // All neighboring tiles have to be hill or beach or gras
                    for(glm::ivec2 offset : GetNeighborOffsets())
                    {
                        offset += pos;
                        if(offset.x < 0 || offset.x >= grid.size() || offset.y < 0 ||
                           offset.y >= grid.front().size())
                        {
                            continue; // Out of bounds
                        }

                        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                                grid[offset.x][offset.y]->getAllPossibleFieldTypes();

                        const bool canBeBeach = std::find(
                                                        possibleFieldTypesOfNeighborField.begin(),
                                                        possibleFieldTypesOfNeighborField.end(),
                                                        BeachFieldDataStruct()
                                                ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeHill = std::find(
                                                       possibleFieldTypesOfNeighborField.begin(),
                                                       possibleFieldTypesOfNeighborField.end(),
                                                       HillFieldDataStruct()
                                               ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeGras = std::find(
                                                       possibleFieldTypesOfNeighborField.begin(),
                                                       possibleFieldTypesOfNeighborField.end(),
                                                       GrasFieldDataStruct()
                                               ) != possibleFieldTypesOfNeighborField.end();

                        if(!canBeGras && !canBeHill && !canBeBeach)
                        {
                            return false;
                        }
                    }

                    return true;
                } }
      )
{
}

HillFieldDataStruct::HillFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::hill,
              2,
              { [&](const glm::ivec2& pos, const grid2d& grid) -> bool
                {
                    // All neighboring tiles have to be hill or gras or mountain
                    for(glm::ivec2 offset : GetNeighborOffsets())
                    {
                        offset += pos;
                        if(offset.x < 0 || offset.x >= grid.size() || offset.y < 0 ||
                           offset.y >= grid.front().size())
                        {
                            continue; // Out of bounds
                        }

                        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                                grid[offset.x][offset.y]->getAllPossibleFieldTypes();

                        const bool canBeMountain = std::find(
                                                           possibleFieldTypesOfNeighborField.begin(),
                                                           possibleFieldTypesOfNeighborField.end(),
                                                           MountainFieldDataStruct()
                                                   ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeHill = std::find(
                                                       possibleFieldTypesOfNeighborField.begin(),
                                                       possibleFieldTypesOfNeighborField.end(),
                                                       HillFieldDataStruct()
                                               ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeGras = std::find(
                                                       possibleFieldTypesOfNeighborField.begin(),
                                                       possibleFieldTypesOfNeighborField.end(),
                                                       GrasFieldDataStruct()
                                               ) != possibleFieldTypesOfNeighborField.end();

                        if(!canBeGras && !canBeHill && !canBeMountain)
                        {
                            return false;
                        }
                    }

                    return true;
                } }
      )
{
}

MountainFieldDataStruct::MountainFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::mountain,
              1,
              { [&](const glm::ivec2& pos, const grid2d& grid) -> bool
                {
                    // All neighboring tiles have to be hill or gras or mountain
                    for(glm::ivec2 offset : GetNeighborOffsets())
                    {
                        offset += pos;
                        if(offset.x < 0 || offset.x >= grid.size() || offset.y < 0 ||
                           offset.y >= grid.front().size())
                        {
                            continue; // Out of bounds
                        }

                        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                                grid[offset.x][offset.y]->getAllPossibleFieldTypes();

                        const bool canBeMountain = std::find(
                                                           possibleFieldTypesOfNeighborField.begin(),
                                                           possibleFieldTypesOfNeighborField.end(),
                                                           MountainFieldDataStruct()
                                                   ) != possibleFieldTypesOfNeighborField.end();

                        const bool canBeHill = std::find(
                                                       possibleFieldTypesOfNeighborField.begin(),
                                                       possibleFieldTypesOfNeighborField.end(),
                                                       HillFieldDataStruct()
                                               ) != possibleFieldTypesOfNeighborField.end();

                        if(!canBeHill && !canBeMountain)
                        {
                            return false;
                        }
                    }

                    return true;
                } }
      )
{
}
