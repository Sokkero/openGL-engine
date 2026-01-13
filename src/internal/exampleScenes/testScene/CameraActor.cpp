#include "CameraActor.h"

#include "classes/engine/EngineManager.h"
#include "classes/engine/UserEventManager.h"

void CameraActor::start()
{
    m_engineManager = SingletonManager::get<Engine::EngineManager>();

    const auto& wCallback = ([this]()
                             { rotateObj(glm::vec3(-1.f, 0.f, 0.f), m_engineManager->getDeltaTime() * 100); });
    const auto& sCallback = ([this]()
                             { rotateObj(glm::vec3(1.f, 0.f, 0.f), m_engineManager->getDeltaTime() * 100); });
    const auto& aCallback = ([this]()
                             { rotateObj(glm::vec3(0.f, 1.f, 0.f), m_engineManager->getDeltaTime() * 100); });
    const auto& dCallback = ([this]()
                             { rotateObj(glm::vec3(0.f, -1.f, 0.f), m_engineManager->getDeltaTime() * 100); });

    const auto& eCallback = ([this]() { moveObj((getForward() * m_engineManager->getDeltaTime()) * 10.f); });
    const auto& qCallback = ([this]() { moveObj((getBackwards() * m_engineManager->getDeltaTime()) * 10.f); });

    const auto& userEventManager = SingletonManager::get<Engine::UserEventManager>();
    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_W, GLFW_PRESS), wCallback);
    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_W, GLFW_REPEAT), wCallback);

    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_S, GLFW_PRESS), sCallback);
    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_S, GLFW_REPEAT), sCallback);

    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_A, GLFW_PRESS), aCallback);
    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_A, GLFW_REPEAT), aCallback);

    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_D, GLFW_PRESS), dCallback);
    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_D, GLFW_REPEAT), dCallback);

    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_E, GLFW_PRESS), eCallback);
    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_E, GLFW_REPEAT), eCallback);

    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_Q, GLFW_PRESS), qCallback);
    userEventManager->addListener(std::pair<int, int>(GLFW_KEY_Q, GLFW_REPEAT), qCallback);
}

void CameraActor::update() {}