
#include "src/classes/engine/GameInterface.h"
#include "src/classes/engine/RenderManager.h"
#include "src/classes/engine/LightingHelper.h"
#include "src/classes/VirtualObjects/SceneOrigin.h"

int main()
{
    std::shared_ptr<Engine::GameInterface> game = std::make_shared<Engine::GameInterface>();

    game->getEngineManager()->getRenderManager()->registerShader("resources/shader/solidColor", "solidColor");
    game->getEngineManager()->getRenderManager()->registerShader("resources/shader/solidTexture", "solidTexture");

    game->getEngineManager()->getRenderManager()->clearShader();

    game->getEngineManager()->getRenderManager()->registerShader("resources/shader/solidColor", "solidColor");
    game->getEngineManager()->getRenderManager()->registerShader("resources/shader/solidTexture", "solidTexture");

    auto& ambientLight = game->getEngineManager()->getRenderManager()->getAmbientLight();
    ambientLight->setIntensity(0.1f);
    ambientLight->setColor(glm::vec3(1.f, 1.f, 1.f));
    ambientLight->UpdateUbo();

    auto& diffuseLight = game->getEngineManager()->getRenderManager()->getDiffuseLight();
    diffuseLight->setIntensity(0.7f);
    diffuseLight->setDir(glm::vec3(1.f, 0.25f, 1.f));
    diffuseLight->setColor(glm::vec3(1.0f, 0.7f, 0.7f));
    diffuseLight->UpdateUbo();

    std::shared_ptr<SceneOrigin> startNode = std::make_shared<SceneOrigin>();
    startNode->setName("Scene Origin");
    game->getEngineManager()->setScene(startNode);

    game->getWindowManager()->setVsync(true);

    return game->startGame();
}
