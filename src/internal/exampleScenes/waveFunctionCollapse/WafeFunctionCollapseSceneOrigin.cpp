#include "WafeFunctionCollapseSceneOrigin.h"

#include "classes/engine/EngineManager.h"
#include "classes/engine/WindowManager.h"
#include "classes/utils/DebugUtils.h"
#include "classes/primitives/actors/CameraActor.h"
#include "classes/primitives/ui/DebugManagerWindow.h"
#include "exampleScenes/waveFunctionCollapse/IslandGenerator.h"
#include "resources/shader/colorShader/ColorShader.h"

WafeFunctionCollapseSceneOrigin::WafeFunctionCollapseSceneOrigin() {}

void WafeFunctionCollapseSceneOrigin::start()
{
    const glm::ivec2 gridDimension = glm::ivec2(20, 20);

    const auto& engineManager = SingletonManager::get<Engine::EngineManager>();
    engineManager->getRenderManager()->getAmbientLightUbo()->setIntensity(.7f);
    SingletonManager::get<Engine::WindowManager>()->setWindowDimensions(700, 700);

    std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    addChild(debugWindow);

    std::shared_ptr<CameraActor> camera = std::make_shared<CameraActor>();
    const float cameraDistance = gridDimension.x > gridDimension.y ? 2.5f * (float)gridDimension.x
                                                                   : 2.5f * (float)gridDimension.y;
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, cameraDistance, 0.f));
    camera->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    addChild(camera);
    engineManager->setCamera(camera);

    auto islandGenerator = std::make_shared<IslandGenerator>(gridDimension);
    addChild(islandGenerator);
}
