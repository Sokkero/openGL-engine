
#include "WafeFunctionCollapseSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/ColorShader.h"
#include "../../classes/helper/DebugUtils.h"

WafeFunctionCollapseSceneOrigin::WafeFunctionCollapseSceneOrigin(const glm::ivec2& fieldDimension)
    : m_field(fieldDimension.x, std::vector<std::shared_ptr<FieldTile>>(fieldDimension.y))
    , m_fieldDimensions(fieldDimension)
{
    FieldTile::setFieldSize(fieldDimension);

    for(int x = 0; x < fieldDimension.x; ++x)
    {
        for(int y = 0; y < fieldDimension.y; ++y)
        {
            m_field[x][y] = std::make_shared<FieldTile>();
            m_field[x][y]->setPosition(glm::ivec2(x, y));
        }
    }
}

void WafeFunctionCollapseSceneOrigin::start()
{
    // Works, nice!
    // But performance is fucking dog shit - Update: less shit now
    const double startTime = glfwGetTime();

    setupScene();
    setupField();

    const double elapsedTime = glfwGetTime() - startTime;

    std::cout << "\nFunction took:" << std::endl;
    DebugUtils::PrintHumanReadableTimeDuration(elapsedTime);
}

void WafeFunctionCollapseSceneOrigin::addDefaultTiles(const bool waterOnEdges, const bool landInMiddle, const uint8_t landTilesToAdd)
{
    const TileTypeEnum waterTile = TileTypeEnum::deepWater;
    const TileTypeEnum landTile = TileTypeEnum::mountain;

    if(waterOnEdges) {
        for(int x = 0; x < m_fieldDimensions.y; ++x)
        {
            addPlane(glm::ivec2(x, 0), waterTile);
            addPlane(glm::ivec2(x, m_fieldDimensions.y - 1), waterTile);
        }

        for(int y = 0; y < m_fieldDimensions.y; ++y)
        {
            addPlane(glm::ivec2(0, y), waterTile);
            addPlane(glm::ivec2(m_fieldDimensions.x - 1, y), waterTile);
        }
    }

    if(landInMiddle) {
        for(int i = 0; i < landTilesToAdd; ++i) {
            const glm::ivec2 tilePos = getTileForTileType(landTile);
            if(tilePos == glm::ivec2(-1.f, -1.f))
            {
                std::cout << "Cant add more land tiles!" << std::endl;
                break;
            }

            addPlane(tilePos, landTile);
        }
    }
}

void WafeFunctionCollapseSceneOrigin::setupField()
{
    std::srand(time(nullptr));

    addDefaultTiles(true, true, (int)(((float)m_fieldDimensions.x * (float)m_fieldDimensions.y) * 0.005f));

    while(true)
    {
        const glm::ivec2 nextTilePos = pickNextTile();

        if(nextTilePos == glm::ivec2(-1.f, -1.f))
        {
            break;
        }

        std::vector<TileTypeEnum> possibleTiles =
                m_field[nextTilePos.x][nextTilePos.y]->getAllPossibleTiles();
        assert(!possibleTiles.empty());

        AddTileWeighting(possibleTiles);
        const TileTypeEnum tileChosen = possibleTiles.at(std::rand() % possibleTiles.size());
        addPlane(glm::ivec2(nextTilePos.x, nextTilePos.y), tileChosen);
    }
}

glm::ivec2 WafeFunctionCollapseSceneOrigin::pickNextTile()
{
    std::vector<glm::ivec2> nextPossibleTiles;
    size_t nextPossibleTileAmount = GetAllTiles().size();
    for(int x = 0; x < m_fieldDimensions.x; ++x)
    {
        for(int y = 0; y < m_fieldDimensions.y; ++y)
        {
            const std::shared_ptr<FieldTile>& currentTile = m_field[x][y];
            if(currentTile->getIsPlaced())
            {
                continue; // Tile already taken
            }

            const size_t possibleTiles = currentTile->getAllPossibleTiles().size();
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

glm::ivec2 WafeFunctionCollapseSceneOrigin::getTileForTileType(TileTypeEnum tile)
{
    std::vector<glm::ivec2> possibleTiles;
    for(int x = 0; x < m_fieldDimensions.x; ++x)
    {
        for(int y = 0; y < m_fieldDimensions.y; ++y)
        {
            const std::shared_ptr<FieldTile>& currentTile = m_field[x][y];
            if(currentTile->getIsPlaced())
            {
                continue; // Tile already taken
            }

            const std::vector<TileTypeEnum> possibleTileTypes = currentTile->getAllPossibleTiles();
            assert(!possibleTileTypes.empty());

            const auto& it = std::find(possibleTileTypes.begin(), possibleTileTypes.end(), tile);
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

void WafeFunctionCollapseSceneOrigin::setupScene()
{
    getEngineManager()->getRenderManager()->getAmbientLightUbo()->setIntensity(.7f);
    getWindowManager()->setWindowDimensions(700, 700);

    // std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    // addChild(debugWindow);

    std::shared_ptr<Engine::CameraComponent> camera = std::make_shared<Engine::CameraComponent>();
    const float cameraDistance = m_fieldDimensions.x > m_fieldDimensions.y ? 2.5f * (float)m_fieldDimensions.x
                                                                           : 2.5f * (float)m_fieldDimensions.y;
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, cameraDistance, 0.f));
    camera->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    addChild(camera);
    getEngineManager()->setCamera(camera);
}

void WafeFunctionCollapseSceneOrigin::addPlane(const glm::ivec2& pos, const TileTypeEnum type)
{
    const std::shared_ptr<Engine::GeometryComponent> plane =
            m_field[pos.x][pos.y]->setTile(type, getEngineManager()->getRenderManager(), m_field);
    addChild(plane);
}