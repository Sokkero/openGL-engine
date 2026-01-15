#include "classes/engine/EngineManager.h"
#include "classes/engine/GameInterface.h"
#include "classes/engine/rendering/RenderManager.h"
#include "exampleScenes/mandelbrotScene/MandelbrotSceneOrigin.h"
#include "exampleScenes/testScene/TestSceneOrigin.h"
#include "exampleScenes/waveFunctionCollapse/WafeFunctionCollapseSceneOrigin.h"

using namespace Engine;

int main()
{
    // This file is for showcasing how the engine can be used and is in no way optimized

#ifdef DEBUG
    std::cout << "DEBUG MODE" << std::endl;
#else
    std::cout << "PROD MODE" << std::endl;
#endif

    const std::shared_ptr<GameInterface> game = std::make_shared<GameInterface>();
    const std::shared_ptr<EngineManager> engineManager = SingletonManager::get<EngineManager>();

    auto& ambientLight = engineManager->getRenderManager()->getAmbientLightUbo();
    ambientLight->setIntensity(.2f);
    ambientLight->setColor(glm::vec3(1.f, 1.f, 1.f));

    auto& diffuseLight = engineManager->getRenderManager()->getDiffuseLightUbo();
    diffuseLight->setIntensity(0.7f);
    diffuseLight->setDir(glm::vec3(1.f, 0.25f, 1.f));
    diffuseLight->setColor(glm::vec3(1.0f, 0.7f, 0.7f));

    // std::shared_ptr<TestSceneOrigin> startNode = std::make_shared<TestSceneOrigin>();
    std::shared_ptr<WafeFunctionCollapseSceneOrigin> startNode = std::make_shared<WafeFunctionCollapseSceneOrigin>();
    // std::shared_ptr<MandelbrotSceneOrigin> startNode = std::make_shared<MandelbrotSceneOrigin>();

    startNode->setName("Scene Origin");
    engineManager->setScene(startNode);

    return game->startGame();
}
