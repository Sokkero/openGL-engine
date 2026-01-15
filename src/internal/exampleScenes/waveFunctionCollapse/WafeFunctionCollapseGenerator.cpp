#include "WafeFunctionCollapseGenerator.h"

#include "classes/utils/DebugUtils.h"
#include "classes/utils/MathUtils.h"
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
    if(m_debugMode) std::cout << "WFCA | Using seed " << m_seed << std::endl;
    GRID_SIZE = dimensions;
}

void WafeFunctionCollapseGenerator::initializeGrid()
{
    if(m_initialized)
    {
        std::cout << "WFCA | Failed to initialize grid: Grid already initialized!" << std::endl;
        return;
    }

    if(m_allFieldTypes.empty())
    {
        std::cout << "WFCA | Failed to initialize grid: No field types added!" << std::endl;
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
        DebugUtils::PrintHumanReadableTimeDuration(glfwGetTime() - startTime, "WFCA | Grid initialized in: ");
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
        std::cout << "WFCA | Failed to generate grid: Grid already generated!" << std::endl;
        return;
    }

    if(!m_initialized)
    {
        std::cout << "WFCA | Failed to generate grid: Grid not yet initialized!" << std::endl;
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
        DebugUtils::PrintHumanReadableTimeDuration(glfwGetTime() - startTime, "WFCA | Grid generated in: ");
        DebugUtils::PrintHumanReadableTimeDuration(
                MathUtils::GetSum(m_timeSpentPickingFields),
                "WFCA | Time spent picking fields: "
        );
        DebugUtils::PrintHumanReadableTimeDuration(
                MathUtils::GetSum(m_timeSpentSettingFields),
                "WFCA | Time spent setting fields: "
        );
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
        std::cout << "WFCA | Failed to set field: Grid already generated!" << std::endl;
        return;
    }

    if(!m_initialized)
    {
        std::cout << "WFCA | Failed to set field: Grid not yet initialized!" << std::endl;
        return;
    }

    if(field->getIsFieldSet())
    {
        std::cout << "WFCA | Failed to set field: Field already set!" << std::endl;
        return;
    }

    field->setField(tileType, m_grid);
    setFieldCallback(field, tileType);
}

bool WafeFunctionCollapseGenerator::presetField(const glm::ivec2& pos, const BasicFieldDataStruct& tileType)
{
    if(m_generated)
    {
        std::cout << "WFCA | Failed to preset field: Grid already generated!" << std::endl;
        return false;
    }

    if(!m_initialized)
    {
        std::cout << "WFCA | Failed to preset field: Grid not yet initialized!" << std::endl;
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
        std::cout << "WFCA | Failed to get field for type: Grid not yet initialized!" << std::endl;
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
