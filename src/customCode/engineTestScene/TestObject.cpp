
#include "TestObject.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"

void TestObject::start() {}

void TestObject::update()
{
    /*
    if(getUserEventManager()->getUserEvent(GLFW_KEY_W) > 0)
    {
        getParentNode()->rotateObj(glm::vec3(1.f, 0.f, 0.f), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getForward() * m_engineManager->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_S) > 0)
    {
        getParentNode()->rotateObj(glm::vec3(-1.f, 0.f, 0.f), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getBackwards() * m_engineManager->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_A) > 0)
    {
        getParentNode()->rotateObj(glm::vec3(0.f, 1.f, 0.f), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getLeft() * m_engineManager->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_D) > 0)
    {
        getParentNode()->rotateObj(glm::vec3(0.f, -1.f, 0.f), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getRight() * m_engineManager->getDeltaTime()) * 10.f);
    }
     */
}