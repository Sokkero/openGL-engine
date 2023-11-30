
#include "src/classes/engine/GameInterface.h"
#include "src/classes/engine/rendering/RenderManager.h"
#include "src/classes/engine/rendering/lighting/LightingPoints.h"
#include "src/customCode/testScene/TestSceneOrigin.h"

int main()
{
    // This file is for showcasing how the engine can be used and in no way optimized

#ifdef DEBUG
    std::cout << "DEBUG MODE" << std::endl;
#else
    std::cout << "PROD MODE" << std::endl;
#endif

    std::shared_ptr<Engine::GameInterface> game = std::make_shared<Engine::GameInterface>();

    auto& ambientLight = game->getEngineManager()->getRenderManager()->getAmbientLightUbo();
    ambientLight->setIntensity(.2f);
    ambientLight->setColor(glm::vec3(1.f, 1.f, 1.f));
    ambientLight->UpdateUbo();

    auto& diffuseLight = game->getEngineManager()->getRenderManager()->getDiffuseLightUbo();
    diffuseLight->setIntensity(0.7f);
    diffuseLight->setDir(glm::vec3(1.f, 0.25f, 1.f));
    diffuseLight->setColor(glm::vec3(1.0f, 0.7f, 0.7f));
    diffuseLight->UpdateUbo();

    std::shared_ptr<TestSceneOrigin> startNode = std::make_shared<TestSceneOrigin>();
    startNode->setName("Scene Origin");
    game->getEngineManager()->setScene(startNode);

    return game->startGame();
}
