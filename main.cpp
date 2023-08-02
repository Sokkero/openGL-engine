
#include "src/classes/engine/GameInterface.h"

#include "src/classes/VirtualObjects/SceneOrigin.h"

int main()
{
    std::shared_ptr<Engine::GameInterface> game = std::make_shared<Engine::GameInterface>();
    std::shared_ptr<SceneOrigin> startNode = std::make_shared<SceneOrigin>();

    game->getEngineManager()->setScene(startNode);

    return game->startGame();
}
