#include "WafeFunctionCollapseGenerator.h"

#include "classes/utils/MathUtils.h"
#include "classes/utils/TimeUtils.h"
#include "exampleScenes/waveFunctionCollapse/Field.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

using namespace Engine;

WafeFunctionCollapseGenerator::WafeFunctionCollapseGenerator(const glm::ivec2& dimensions, const long& seed, const bool debugOutput)
    : m_seed(0)
    , m_generated(false)
    , m_initialized(false)
    , m_debugMode(debugOutput)
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
    if(m_debugMode) LOG_DEBUG("WFCA", stringf("Using seed %s", m_seed));
    GRID_SIZE = dimensions;
}

void WafeFunctionCollapseGenerator::initializeGrid()
{
    if(m_initialized)
    {
        LOG_DEBUG("WFCA", "Failed to initialize grid: Grid already initialized!");
        return;
    }

    if(m_allFieldTypes.empty())
    {
        LOG_DEBUG("WFCA", "Failed to initialize grid: No field types added!");
        return;
    }

    const double startTime = glfwGetTime();
    for(int x = 0; x < GRID_SIZE.x; ++x)
    {
        for(int y = 0; y < GRID_SIZE.y; ++y)
        {
            m_grid[x][y] = std::make_shared<Field>(m_allFieldTypes);
            m_grid[x][y]->setPosition(glm::ivec2(x, y));
        }
    }

    m_initialized = true;
    if(m_debugMode)
    {
        const std::string duration = TimeUtils::GetHumanReadableTimeDuration(glfwGetTime() - startTime);
        LOG_DEBUG("WFCA", stringf("Grid initialized in: %s", duration));
    }
}

void WafeFunctionCollapseGenerator::addFieldTypes(const std::vector<BasicFieldDataStruct>& fieldTypes)
{
    for(const BasicFieldDataStruct& type : fieldTypes)
    {
        if(std::find(m_allFieldTypes.begin(), m_allFieldTypes.end(), type) == m_allFieldTypes.end())
        {
            m_allFieldTypes.push_back(type);
        }
    }
}

void WafeFunctionCollapseGenerator::generateGrid()
{
    if(m_generated)
    {
        LOG_DEBUG("WFCA", "Failed to generate grid: Grid already generated!");
        return;
    }

    if(!m_initialized)
    {
        LOG_DEBUG("WFCA", "Failed to generate grid: Grid not yet initialized!");
        return;
    }

    const double startTime = glfwGetTime();
    while(true)
    {
        if(!generateNextField())
        {
            break;
        }
    }

    m_generated = true;
    if(m_debugMode)
    {
        const std::string gridTime = TimeUtils::GetHumanReadableTimeDuration(glfwGetTime() - startTime);
        const std::string pickTime = TimeUtils::GetHumanReadableTimeDuration(MathUtils::GetSum(m_timeSpentPickingFields
        ));
        const std::string setTime = TimeUtils::GetHumanReadableTimeDuration(MathUtils::GetSum(m_timeSpentSettingFields
        ));

        LOG_DEBUG("WFCA", stringf("Grid generated in: %s", gridTime));
        LOG_DEBUG("WFCA", stringf("Time spent picking fields: %s", pickTime));
        LOG_DEBUG("WFCA", stringf("Time spent setting fields: %s", setTime));
    }
}

bool WafeFunctionCollapseGenerator::generateNextField()
{
    double startTime = glfwGetTime();
    const std::shared_ptr<Field>& nextField = pickNextField();
    if(m_debugMode)
    {
        m_timeSpentPickingFields.push_back(glfwGetTime() - startTime);
    }

    if(!nextField)
    {
        return false;
    }

    std::vector<BasicFieldDataStruct> possibleFieldTypes = nextField->getAllPossibleFieldTypes();
    assert(!possibleFieldTypes.empty());

    AddFieldWeighting(possibleFieldTypes);
    const BasicFieldDataStruct tileChosen = possibleFieldTypes.at(std::rand() % possibleFieldTypes.size());

    startTime = glfwGetTime();
    setField(nextField, tileChosen);
    if(m_debugMode)
    {
        m_timeSpentSettingFields.push_back(glfwGetTime() - startTime);
    }

    return true;
}

void WafeFunctionCollapseGenerator::setField(const std::shared_ptr<Field>& field, const BasicFieldDataStruct& tileType)
{
    if(m_generated)
    {
        LOG_WARN("WFCA", "Failed to set field: Grid already generated!");
        return;
    }

    if(!m_initialized)
    {
        LOG_WARN("WFCA", "Failed to set field: Grid not yet initialized!");
        return;
    }

    if(field->getIsFieldSet())
    {
        LOG_WARN("WFCA", "Failed to set field: Field already set!");
        return;
    }

    field->setField(tileType, m_grid);
    setFieldCallback(field, tileType);
}

bool WafeFunctionCollapseGenerator::presetField(const glm::ivec2& pos, const BasicFieldDataStruct& tileType)
{
    if(m_generated)
    {
        LOG_WARN("WFCA", "Failed to preset field: Grid already generated!");
        return false;
    }

    if(!m_initialized)
    {
        LOG_WARN("WFCA", "Failed to preset field: Grid not yet initialized!");
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

const std::shared_ptr<Field> WafeFunctionCollapseGenerator::pickNextField() const
{
    std::vector<std::shared_ptr<Field>> nextPossibleTiles;
    size_t nextPossibleTileAmount = m_allFieldTypes.size();
    std::mutex mtx;
    std::vector<std::thread> threads;
    for(const auto& row : m_grid)
    {
        threads.emplace_back((
                [&]()
                {
                    for(const auto& field : row)
                    {
                        if(field->getIsFieldSet())
                        {
                            continue; // Tile already taken
                        }

                        const size_t possibleTiles = field->getAllPossibleFieldTypes().size();
                        assert(possibleTiles > 0);

                        if(possibleTiles == nextPossibleTileAmount)
                        {
                            mtx.lock();
                            nextPossibleTiles.push_back(field);
                            mtx.unlock();
                        }
                        else if(possibleTiles < nextPossibleTileAmount)
                        {
                            mtx.lock();
                            nextPossibleTileAmount = possibleTiles;
                            nextPossibleTiles.clear();
                            nextPossibleTiles.push_back(field);
                            mtx.unlock();
                        }
                    }
                }
        ));
    }

    for(auto& thread : threads)
    {
        thread.join();
    }

    if(nextPossibleTiles.empty())
    {
        return nullptr;
    }

    return nextPossibleTiles.at(std::rand() % nextPossibleTiles.size());
}

const glm::ivec2 WafeFunctionCollapseGenerator::getFieldForFieldType(const BasicFieldDataStruct& tileType) const
{
    if(!m_initialized)
    {
        LOG_WARN("WFCA", "Failed to get field for type: Grid not yet initialized!");
        return glm::ivec2(-1.f, -1.f);
    }

    if(m_generated)
    {
        return glm::ivec2(-1.f, -1.f);
    }

    std::vector<glm::ivec2> possibleTiles;
    for(int x = 0; x < GRID_SIZE.x; ++x)
    {
        for(int y = 0; y < GRID_SIZE.y; ++y)
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
