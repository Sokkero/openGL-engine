
#include "WafeFunctionCollapseSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/ColorShader.h"

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
    // But performance is fucking dog shit
    const double startTime = glfwGetTime();

    setupScene();
    setupField();

    const double elapsedTime = glfwGetTime() - startTime;

    const int hours = static_cast<int>(elapsedTime) / 3600;
    const int minutes = (static_cast<int>(elapsedTime) % 3600) / 60;
    const int seconds = static_cast<int>(elapsedTime) % 60;
    const int milliseconds = static_cast<int>((elapsedTime - static_cast<int>(elapsedTime)) * 1000);

    std::cout << "Function took: "
              << hours << "h "
              << minutes << "m "
              << seconds << "s "
              << milliseconds << "ms" << std::endl;
}

void WafeFunctionCollapseSceneOrigin::setupField()
{
    std::srand(time(nullptr));

    for(const auto& predeterminedTile : GetPredeterminedTiles())
    {
        addPlane(glm::ivec2(predeterminedTile.first.x, predeterminedTile.first.y), predeterminedTile.second);
    }

    bool canContinue = true;
    while(canContinue)
    {
        updateAllTiles();

        const glm::ivec2 nextTilePos = pickNextTile();

        if(nextTilePos == glm::ivec2(-1.f, -1.f))
        {
            canContinue = false;
            continue;
        }

        const std::vector<TileTypeEnum> possibleTiles =
                m_field[nextTilePos.x][nextTilePos.y]->getAllPossibleTiles();
        if(possibleTiles.empty())
        {
            addPlane(glm::ivec2(nextTilePos.x, nextTilePos.y), TileTypeEnum::undetermined);
            return;
        }

        const TileTypeEnum tileChosen = possibleTiles.at(std::rand() % possibleTiles.size());
        addPlane(glm::ivec2(nextTilePos.x, nextTilePos.y), tileChosen);
    }
}

void WafeFunctionCollapseSceneOrigin::updateAllTiles()
{
    bool hasUpdated = true;
    while(hasUpdated)
    {
        hasUpdated = false;
        for(const auto& row : m_field)
        {
            for(const auto& tile : row)
            {
                if(tile->getCurrentTile() == TileTypeEnum::undetermined)
                {
                    tile->updatePossibleTiles(m_field, hasUpdated);
                }
            }
        }
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
            if(currentTile->getCurrentTile() != TileTypeEnum::undetermined)
            {
                continue; // Tile already taken
            }

            const size_t possibleTiles = currentTile->getAllPossibleTiles().size();
            // assert(possibleTiles > 0);

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
            m_field[pos.x][pos.y]->setTile(type, getEngineManager()->getRenderManager());
    addChild(plane);
}