
#include "WafeFunctionCollapseSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/helper/DebugUtils.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/ColorShader.h"

WafeFunctionCollapseSceneOrigin::WafeFunctionCollapseSceneOrigin() {}

void WafeFunctionCollapseSceneOrigin::start()
{
    const glm::ivec2 gridDimension = glm::ivec2(30, 30);

    getEngineManager()->getRenderManager()->getAmbientLightUbo()->setIntensity(.7f);
    getWindowManager()->setWindowDimensions(700, 700);

    // std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    // addChild(debugWindow);

    std::shared_ptr<Engine::CameraComponent> camera = std::make_shared<Engine::CameraComponent>();
    const float cameraDistance = gridDimension.x > gridDimension.y ? 2.5f * (float)gridDimension.x
                                                                   : 2.5f * (float)gridDimension.y;
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, cameraDistance, 0.f));
    camera->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    addChild(camera);
    getEngineManager()->setCamera(camera);
}

/*
void WafeFunctionCollapseSceneOrigin::addDefaultTiles(const bool waterOnEdges, const bool landInMiddle, const uint8_t landTilesToAdd)
{
    const FieldTypeEnum waterTile = FieldTypeEnum::deepWater;
    const FieldTypeEnum landTile = FieldTypeEnum::mountain;

    if(waterOnEdges)
    {
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

    if(landInMiddle)
    {
        for(int i = 0; i < landTilesToAdd; ++i)
        {
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
 */
