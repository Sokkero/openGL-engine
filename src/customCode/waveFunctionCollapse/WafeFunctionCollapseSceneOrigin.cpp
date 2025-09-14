
#include "WafeFunctionCollapseSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/helper/DebugUtils.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/ColorShader.h"
#include "IslandGenerator.h"

WafeFunctionCollapseSceneOrigin::WafeFunctionCollapseSceneOrigin() {}

void WafeFunctionCollapseSceneOrigin::start()
{
    const glm::ivec2 gridDimension = glm::ivec2(100, 100);

    const auto& engineManager = SingletonManager::get<Engine::EngineManager>();
    engineManager->getRenderManager()->getAmbientLightUbo()->setIntensity(.7f);
    SingletonManager::get<Engine::WindowManager>()->setWindowDimensions(700, 700);

    std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    addChild(debugWindow);

    std::shared_ptr<Engine::CameraComponent> camera = std::make_shared<Engine::CameraComponent>();
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
