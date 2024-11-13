
#include "IslandGenerator.h"

#include "../../classes/nodeComponents/GeometryComponent.h"
#include "../../resources/shader/ColorShader.h"
#include "CustomFieldTypeData.h"
#include "Field.h"

IslandGenerator::IslandGenerator(const glm::ivec2& gridDimensions, const double& seed)
    : WafeFunctionCollapseGenerator(gridDimensions, seed)
{
    FIELD_SIZE = glm::vec2(2.f);
}

void IslandGenerator::start()
{
    addFieldTypes({ DeepWaterFieldDataStruct(),
                    ShallowWaterFieldDataStruct(),
                    BeachFieldDataStruct(),
                    GrasFieldDataStruct(),
                    HillFieldDataStruct(),
                    MountainFieldDataStruct() });
    initializeGrid();
    addDefaultTiles(true, true, (int)(((float)GRID_SIZE.x * (float)GRID_SIZE.y) * 0.005f));
    generateGrid();
}

void IslandGenerator::setFieldCallback(const glm::ivec2& pos, const BasicFieldDataStruct& tileType)
{
    static const float startPosX = (FIELD_SIZE.x * ((float)GRID_SIZE.x - 1.f)) / 2.f;
    static const float startPosY = (FIELD_SIZE.y * ((float)GRID_SIZE.y - 1.f)) / 2.f;

    const float posX = (pos.x * FIELD_SIZE.x) - startPosX;
    const float posY = (pos.y * FIELD_SIZE.y) - startPosY;

    const std::shared_ptr<Engine::RenderManager> renderManager = getEngineManager()->getRenderManager();

    std::shared_ptr<Engine::GeometryComponent> planeObj = std::make_shared<Engine::GeometryComponent>();
    planeObj->setObjectData(getEngineManager()->getRenderManager()->registerObject("resources/objects/plane.obj"));
    planeObj->setShader(std::make_shared<ColorShader>(renderManager));
    planeObj->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    planeObj->setPosition(glm::vec3(posX, 0.f, posY));

    std::vector<glm::vec4> g_color_buffer_data;
    const glm::vec3 color = EnumToColorValue(tileType.uniqueTileTypeId);
    for(int v = 0; v < planeObj->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(color, 1.f);
    }

    planeObj->setTextureBuffer(renderManager->createBuffer(g_color_buffer_data));

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
            setField(glm::ivec2(x, 0), waterTile);
            setField(glm::ivec2(x, GRID_SIZE.y - 1), waterTile);
        }

        for(int y = 0; y < GRID_SIZE.y; ++y)
        {
            setField(glm::ivec2(0, y), waterTile);
            setField(glm::ivec2(GRID_SIZE.x - 1, y), waterTile);
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

            setField(tilePos, landTile);
        }
    }
}
