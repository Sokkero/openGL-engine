
#include "src/classes/engine/GameInterface.h"
#include "src/classes/engine/RenderManager.h"
#include "src/classes/engine/LightingHelper.h"
#include "src/classes/VirtualObjects/SceneOrigin.h"

int main()
{
    std::shared_ptr<Engine::GameInterface> game = std::make_shared<Engine::GameInterface>();

    auto& ambientLight = game->getEngineManager()->getRenderManager()->getAmbientLight();
    ambientLight->setIntensity(0.2f);
    ambientLight->UpdateUbo();

    auto& diffuseLight = game->getEngineManager()->getRenderManager()->getDiffuseLight();
    diffuseLight->setUseDiffuse(true);
    diffuseLight->setIntensity(1.0f);
    diffuseLight->UpdateUbo();

    std::shared_ptr<SceneOrigin> startNode = std::make_shared<SceneOrigin>();
    startNode->setName("Scene Origin");
    game->getEngineManager()->setScene(startNode);

    return game->startGame();
}
