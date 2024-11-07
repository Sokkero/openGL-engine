#include "CameraActor.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"

void CameraActor::start()
{
    const auto& wCallback =
            ([this]() { rotateObj(glm::vec3(-1.f, 0.f, 0.f), getEngineManager()->getDeltaTime() * 100); });
    const auto& sCallback =
            ([this]() { rotateObj(glm::vec3(1.f, 0.f, 0.f), getEngineManager()->getDeltaTime() * 100); });
    const auto& aCallback =
            ([this]() { rotateObj(glm::vec3(0.f, 1.f, 0.f), getEngineManager()->getDeltaTime() * 100); });
    const auto& dCallback =
            ([this]() { rotateObj(glm::vec3(0.f, -1.f, 0.f), getEngineManager()->getDeltaTime() * 100); });

    const auto& eCallback = ([this]() { moveObj((getForward() * getEngineManager()->getDeltaTime()) * 10.f); });
    const auto& qCallback = ([this]()
                             { moveObj((getBackwards() * getEngineManager()->getDeltaTime()) * 10.f); });

    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_W, GLFW_PRESS), wCallback);
    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_W, GLFW_REPEAT), wCallback);

    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_S, GLFW_PRESS), sCallback);
    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_S, GLFW_REPEAT), sCallback);

    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_A, GLFW_PRESS), aCallback);
    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_A, GLFW_REPEAT), aCallback);

    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_D, GLFW_PRESS), dCallback);
    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_D, GLFW_REPEAT), dCallback);

    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_E, GLFW_PRESS), eCallback);
    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_E, GLFW_REPEAT), eCallback);

    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_Q, GLFW_PRESS), qCallback);
    getUserEventManager()->addListener(std::pair<int, int>(GLFW_KEY_Q, GLFW_REPEAT), qCallback);
}

void CameraActor::update() {}