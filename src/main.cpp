
#include "classes/engine/GameInterface.h"
#include "classes/engine/rendering/RenderManager.h"
#include "customCode/mandelbrotScene/MandelbrotSceneOrigin.h"
#include "customCode/testScene/TestSceneOrigin.h"
#include "customCode/waveFunctionCollapse/WafeFunctionCollapseSceneOrigin.h"

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

    diffuseLight->setIsActive(false);

    // std::shared_ptr<TestSceneOrigin> startNode = std::make_shared<TestSceneOrigin>();
    std::shared_ptr<WafeFunctionCollapseSceneOrigin> startNode =
            std::make_shared<WafeFunctionCollapseSceneOrigin>(glm::ivec2(100, 100));
    // std::shared_ptr<MandelbrotSceneOrigin> startNode = std::make_shared<MandelbrotSceneOrigin>();
    startNode->setName("Scene Origin");
    game->getEngineManager()->setScene(startNode);

    return game->startGame();
}
