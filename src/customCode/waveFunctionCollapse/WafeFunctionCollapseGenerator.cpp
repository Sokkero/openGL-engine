
#include "WafeFunctionCollapseGenerator.h"

#include "Field.h"

#include <iostream>

WafeFunctionCollapseGenerator::WafeFunctionCollapseGenerator(const glm::ivec2& dimensions, const long& seed)
    : m_gridDimensions(dimensions)
    , m_seed(0)
    , m_generated(false)
    , m_initialized(false)
    , m_grid(dimensions.x, std::vector<std::shared_ptr<Field>>(dimensions.y))
{
    if(seed == 0)
    {
        m_seed = time(nullptr);
    }
    else
    {
        m_seed = seed;
    }

    std::srand(m_seed);
    Field::setGridSize(m_gridDimensions);
}

void WafeFunctionCollapseGenerator::initializeGrid()
{
    if(m_initialized)
    {
        return;
    }

    for(int x = 0; x < m_gridDimensions.x; ++x)
    {
        for(int y = 0; y < m_gridDimensions.y; ++y)
        {
            m_grid[x][y] = std::make_shared<Field>(m_fieldTypes);
            m_grid[x][y]->setPosition(glm::ivec2(x, y));
        }
    }

    m_initialized = true;
}

void WafeFunctionCollapseGenerator::addFieldTypes(const std::vector<BasicFieldDataStruct>& fieldTypes)
{
    for(const BasicFieldDataStruct& type : fieldTypes)
    {
        if(std::find(m_fieldTypes.begin(), m_fieldTypes.end(), type) == m_fieldTypes.end())
        {
            m_fieldTypes.push_back(type);
        }
    }
}

void WafeFunctionCollapseGenerator::generateGrid()
{
    if(m_generated)
    {
        return;
    }

    while(true)
    {
        const glm::ivec2 nextTilePos = pickNextField();

        if(nextTilePos == glm::ivec2(-1.f, -1.f))
        {
            break;
        }

        std::vector<BasicFieldDataStruct> possibleTiles =
                m_grid[nextTilePos.x][nextTilePos.y]->getAllPossibleFieldTypes();
        assert(!possibleTiles.empty());

        // AddFieldWeighting(possibleTiles);
        const BasicFieldDataStruct tileChosen = possibleTiles.at(std::rand() % possibleTiles.size());
        setField(nextTilePos, tileChosen);
    }

    m_generated = true;
}

bool WafeFunctionCollapseGenerator::presetField(const glm::ivec2& pos, const BasicFieldDataStruct& tileType)
{
    if(m_generated)
    {
        std::cout << "Failed to preset field: Grid already generated!" << std::endl;
        return false;
    }

    if(!m_initialized)
    {
        std::cout << "Failed to preset field: Grid not yet initialized!" << std::endl;
        return false;
    }

    const std::shared_ptr<Field>& field = m_grid[pos.x][pos.y];
    for(const auto& type : field->getAllPossibleFieldTypes())
    {
        if(type != tileType)
        {
            continue;
        }
        field->setField(tileType, m_grid);
        return true;
    }
}

const glm::ivec2 WafeFunctionCollapseGenerator::pickNextField() const
{
    std::vector<glm::ivec2> nextPossibleTiles;
    size_t nextPossibleTileAmount = m_fieldTypes.size();
    for(int x = 0; x < m_gridDimensions.x; ++x)
    {
        for(int y = 0; y < m_gridDimensions.y; ++y)
        {
            const std::shared_ptr<Field>& currentTile = m_grid[x][y];
            if(currentTile->getIsFieldSet())
            {
                continue; // Tile already taken
            }

            const size_t possibleTiles = currentTile->getAllPossibleFieldTypes().size();
            assert(possibleTiles > 0);

            if(possibleTiles == 1)
            {
                return glm::ivec2(x, y);
            }

            if(possibleTiles == nextPossibleTileAmount)
            {
                nextPossibleTiles.push_back(glm::ivec2(x, y));
            }
            else if(possibleTiles < nextPossibleTileAmount)
            {
                nextPossibleTileAmount = possibleTiles;
                nextPossibleTiles.clear();
                nextPossibleTiles.push_back(glm::ivec2(x, y));
            }
        }
    }

    if(nextPossibleTiles.empty())
    {
        return glm::ivec2(-1, -1);
    }

    return nextPossibleTiles.at(std::rand() % nextPossibleTiles.size());
}

const glm::ivec2 WafeFunctionCollapseGenerator::getFieldForFieldType(const BasicFieldDataStruct& tileType) const
{
    if(m_generated)
    {
        return glm::ivec2(-1.f, -1.f);
    }

    std::vector<glm::ivec2> possibleTiles;
    for(int x = 0; x < m_gridDimensions.x; ++x)
    {
        for(int y = 0; y < m_gridDimensions.y; ++y)
        {
            const std::shared_ptr<Field>& currentTile = m_grid[x][y];
            if(currentTile->getIsFieldSet())
            {
                continue; // Tile already taken
            }

            const std::vector<BasicFieldDataStruct> possibleTileTypes = currentTile->getAllPossibleFieldTypes();
            assert(!possibleTileTypes.empty());

            const auto& it = std::find(possibleTileTypes.begin(), possibleTileTypes.end(), tileType);
            if(it != possibleTileTypes.end())
            {
                possibleTiles.push_back(glm::ivec2(x, y));
            }
        }
    }

    if(possibleTiles.empty())
    {
        return glm::ivec2(-1, -1);
    }

    return possibleTiles.at(std::rand() % possibleTiles.size());
}
