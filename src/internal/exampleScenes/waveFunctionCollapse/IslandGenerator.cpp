#include "IslandGenerator.h"

#include "classes/engine/UserEventManager.h"
#include "classes/helper/RenderUtils.h"
#include "classes/nodeComponents/GeometryComponent.h"
#include "exampleScenes/waveFunctionCollapse/CustomFieldTypeData.h"
#include "exampleScenes/waveFunctionCollapse/Field.h"
#include "resources/shader/colorShader/ColorShader.h"

IslandGenerator::IslandGenerator(const glm::ivec2& gridDimensions, const double& seed)
    : WafeFunctionCollapseGenerator(gridDimensions, seed, true)
{
    FIELD_SIZE = glm::vec2(2.f);
}

void IslandGenerator::start()
{
    addFieldTypes({ DeepWaterFieldDataStruct(),
                    ShallowWaterFieldDataStruct(),
                    BeachFieldDataStruct(),
                    GrasFieldDataStruct(),
                    StoneFieldDataStruct(),
                    HillFieldDataStruct(),
                    MountainFieldDataStruct() });
    initializeGrid();
    addDefaultTiles(true, true, (int)(((float)GRID_SIZE.x * (float)GRID_SIZE.y) * 0.005f));
    generateGrid();

    // getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_SPACE, GLFW_PRESS), ([this]() { generateNextField(); }));
}

void IslandGenerator::setFieldCallback(const std::shared_ptr<Field>& field, const BasicFieldDataStruct& tileType)
{
    static const float startPosX = (FIELD_SIZE.x * ((float)GRID_SIZE.x - 1.f)) / 2.f;
    static const float startPosY = (FIELD_SIZE.y * ((float)GRID_SIZE.y - 1.f)) / 2.f;

    const glm::ivec2 fieldPos = field->getPosition();
    const float posX = (fieldPos.x * FIELD_SIZE.x) - startPosX;
    const float posY = (fieldPos.y * FIELD_SIZE.y) - startPosY;

    const auto& engineManager = SingletonManager::get<Engine::EngineManager>();
    const auto& renderManager = engineManager->getRenderManager();

    std::shared_ptr<Engine::GeometryComponent> planeObj = std::make_shared<Engine::GeometryComponent>();
    planeObj->setObjectData(renderManager->registerObject("resources/objects/plane.obj"));
    planeObj->setShader(std::make_shared<Engine::ColorShader>(renderManager));
    planeObj->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    planeObj->setPosition(glm::vec3(posX, 0.f, posY));

    std::vector<glm::vec4> g_color_buffer_data;
    const glm::vec3 color = EnumToColorValue(tileType.uniqueTileTypeId);
    for(int v = 0; v < planeObj->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(color, 1.f);
    }

    planeObj->setTextureBuffer(Engine::RenderUtils::createVertexBufferObject(g_color_buffer_data));

    addChild(planeObj);
}

void IslandGenerator::addDefaultTiles(const bool waterOnEdges, const bool landInMiddle, const uint8_t landTilesToAdd)
{
    const DeepWaterFieldDataStruct waterTile;
    const MountainFieldDataStruct landTile;

    if(waterOnEdges)
    {
        for(int x = 0; x < GRID_SIZE.y; ++x)
        {
            setField(m_grid[x][0], waterTile);
            setField(m_grid[x][GRID_SIZE.y - 1], waterTile);
        }

        for(int y = 0; y < GRID_SIZE.y; ++y)
        {
            setField(m_grid[0][y], waterTile);
            setField(m_grid[GRID_SIZE.x - 1][y], waterTile);
        }
    }

    if(landInMiddle)
    {
        for(int i = 0; i < landTilesToAdd; ++i)
        {
            const glm::ivec2 tilePos = getFieldForFieldType(landTile);
            if(tilePos == glm::ivec2(-1.f, -1.f))
            {
                std::cout << "Cant add more land tiles!" << std::endl;
                break;
            }

            setField(m_grid[tilePos.x][tilePos.y], landTile);
        }
    }
}
