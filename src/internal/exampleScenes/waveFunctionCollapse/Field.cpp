#include "Field.h"

#include "exampleScenes/waveFunctionCollapse/WafeFunctionCollapseGenerator.h"

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

        if(offset.x < 0 || offset.x >= WafeFunctionCollapseGenerator::GRID_SIZE.x || offset.y < 0
           || offset.y >= WafeFunctionCollapseGenerator::GRID_SIZE.y)
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

    if(m_possibleFieldTypes.size() != possibleFields.size())
    {
        m_possibleFieldTypes = possibleFields;
        updateNeighboringFields(grid);
    }
}

bool Field::getCanBeFieldType(const BasicFieldDataStruct& fieldType) const
{
    for(const BasicFieldDataStruct& possibleFieldType : m_possibleFieldTypes)
    {
        if(fieldType == possibleFieldType)
        {
            return true;
        }
    }
    return false;
}

bool Field::getIsSetAsFieldType(const BasicFieldDataStruct& fieldType) const
{
    if(!m_fieldSet)
    {
        return false;
    }

    return m_possibleFieldTypes.front() == fieldType;
}

void Field::setField(const BasicFieldDataStruct& type, const std::vector<std::vector<std::shared_ptr<Field>>>& grid)
{
    m_possibleFieldTypes.clear();
    m_possibleFieldTypes.push_back(type);
    m_fieldSet = true;

    updateNeighboringFields(grid);
}
