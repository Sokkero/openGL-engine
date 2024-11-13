
#include "Field.h"

#include "WafeFunctionCollapseGenerator.h"

Field::Field(const std::vector<BasicFieldDataStruct>& fieldTypes)
    : m_fieldSet(false)
    , m_fieldPos(glm::ivec2())
    , m_possibleFieldTypes(fieldTypes)
{
}

void Field::updateNeighboringFields(const std::vector<std::vector<std::shared_ptr<Field>>>& grid)
{
    for(glm::ivec2 offset : GetNeighborOffsets())
    {
        offset += m_fieldPos;

        if(offset.x < 0 || offset.x >= WafeFunctionCollapseGenerator::GRID_SIZE.x || offset.y < 0 ||
           offset.y >= WafeFunctionCollapseGenerator::GRID_SIZE.y)
        {
            continue; // Out of bounds
        }

        grid[offset.x][offset.y]->updatePossibleFields(grid);
    }
}

void Field::updatePossibleFields(const std::vector<std::vector<std::shared_ptr<Field>>>& grid)
{
    if(m_possibleFieldTypes.size() == 1)
    {
        return;
    }

    // New
    std::vector<BasicFieldDataStruct> possibleFields = m_possibleFieldTypes;
    for(const auto& field : possibleFields)
    {
        if(possibleFields.size() == 1)
        {
            break;
        }

        if(!BasicFieldDataStruct::checkRulesForPosition(m_fieldPos, grid, field))
        {
            possibleFields.erase(std::find(possibleFields.begin(), possibleFields.end(), field));
        }
    }

    /*
     * Old
    std::vector<BasicFieldDataStruct> possibleFields = m_possibleFieldTypes;
    for(glm::ivec2 offset : GetNeighborOffsets())
    {
        offset += m_fieldPos;

        if(offset.x < 0 || offset.x >= WafeFunctionCollapseGenerator::GRID_SIZE.x || offset.y < 0 || offset.y
    >= WafeFunctionCollapseGenerator::GRID_SIZE.y)
        {
            continue; // Out of bounds
        }

        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                grid[offset.x][offset.y]->getAllPossibleFieldTypes();

        std::vector<BasicFieldDataStruct> fieldsToRemove;
        for(const BasicFieldDataStruct& possibleFieldType : possibleFields)
        {
            bool found = false;
            for(const BasicFieldDataStruct& possibleNeighborFieldType : possibleFieldTypesOfNeighborField)
            {
                auto it = std::find(
                        possibleNeighborFieldType.allowedNeighborTileTypeIds.begin(),
                        possibleNeighborFieldType.allowedNeighborTileTypeIds.end(),
                        possibleFieldType.tileTypeId
                );
                if(it != possibleNeighborFieldType.allowedNeighborTileTypeIds.end())
                {
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                fieldsToRemove.push_back(possibleFieldType);
            }

            if(possibleFields.size() - fieldsToRemove.size() == 1)
            {
                break;
            }
        }

        possibleFields.erase(
                std::remove_if(
                        possibleFields.begin(),
                        possibleFields.end(),
                        [&](const BasicFieldDataStruct& fieldToCheck) {
                            return std::find(fieldsToRemove.begin(), fieldsToRemove.end(), fieldToCheck) !=
                                    fieldsToRemove.end();
                        }
                ),
                possibleFields.end()
        );

        if(possibleFields.size() == 1)
        {
            break;
        }
    }
     */

    if(m_possibleFieldTypes.size() != possibleFields.size())
    {
        m_possibleFieldTypes = possibleFields;
        updateNeighboringFields(grid);
    }
}

void Field::setField(BasicFieldDataStruct type, const std::vector<std::vector<std::shared_ptr<Field>>>& grid)
{
    m_possibleFieldTypes.clear();
    m_possibleFieldTypes.push_back(type);
    m_fieldSet = true;

    updateNeighboringFields(grid);
}
