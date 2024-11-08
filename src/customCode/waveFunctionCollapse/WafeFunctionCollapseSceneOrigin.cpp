
#include "WafeFunctionCollapseSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/ColorShader.h"

WafeFunctionCollapseSceneOrigin::WafeFunctionCollapseSceneOrigin() : m_field()
{
    // Initialize dield array
    for(int x = 0; x < 10; ++x)
    {
        for(int y = 0; y < 10; ++y)
        {
            m_field[x][y] = std::make_shared<FieldTile>();
            m_field[x][y]->setPosition(glm::ivec2(x, y));
        }
    }
}

void WafeFunctionCollapseSceneOrigin::start()
{
    setupScene();
    setupField();
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

        if(nextTilePos == glm::ivec2(-1.f, -1.f)) {
            canContinue = false;
            continue;
        }

        const std::vector<TileTypeEnum> possibleTiles = m_field[nextTilePos.x][nextTilePos.y]->getAllPossibleTiles();
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
    while(hasUpdated) {
        hasUpdated = false;
        for(const auto& row : m_field)
        {
            for(const auto& tile : row)
            {
                if(tile->getCurrentTile() == TileTypeEnum::undetermined) {
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
    for(int x = 0; x < 10; ++x)
    {
        for(int y = 0; y < 10; ++y)
        {
            const std::shared_ptr<FieldTile>& currentTile = m_field[x][y];
            if(currentTile->getCurrentTile() != TileTypeEnum::undetermined)
            {
                continue; // Tile already taken
            }

            const size_t possibleTiles = currentTile->getAllPossibleTiles().size();
            //assert(possibleTiles > 0);

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

    if(nextPossibleTiles.empty()){
        return glm::ivec2(-1, -1);
    }

    return nextPossibleTiles.at(std::rand() % nextPossibleTiles.size());
}

void WafeFunctionCollapseSceneOrigin::setupScene()
{
    getEngineManager()->getRenderManager()->getAmbientLightUbo()->setIntensity(.7f);

    // std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    // addChild(debugWindow);

    getWindowManager()->setWindowDimensions(1024, 768);

    std::shared_ptr<Engine::CameraComponent> camera = std::make_shared<Engine::CameraComponent>();
    camera->setPosition(glm::vec3(0.f, 25.f, 0.f));
    camera->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    addChild(camera);
    getEngineManager()->setCamera(camera);
}

void WafeFunctionCollapseSceneOrigin::addPlane(glm::ivec2 pos, TileTypeEnum type)
{
    const std::shared_ptr<Engine::GeometryComponent> plane = m_field[pos.x][pos.y]->setTile(type, getEngineManager()->getRenderManager());
    addChild(plane);
}