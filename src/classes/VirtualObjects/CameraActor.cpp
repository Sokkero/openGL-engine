#include "CameraActor.h"

#include "../engine/WindowManager.h"
#include "../engine/EngineManager.h"
#include "../engine/UserEventManager.h"

void CameraActor::start()
{
    m_engineManager = getWindowManager()->getEngine();
    m_userEventManager = getWindowManager()->getUserEventManager();
}

void CameraActor::update()
{
    if(m_userEventManager->getUserEvent(GLFW_KEY_W) > 0)
    {
        moveObj((getForward() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(m_userEventManager->getUserEvent(GLFW_KEY_S) > 0)
    {
        moveObj((getBackwards() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(m_userEventManager->getUserEvent(GLFW_KEY_A) > 0)
    {
        moveObj((getLeft() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(m_userEventManager->getUserEvent(GLFW_KEY_D) > 0)
    {
        moveObj((getRight() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(m_userEventManager->getUserEvent(GLFW_KEY_E) > 0)
    {
        moveObj((getUp() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(m_userEventManager->getUserEvent(GLFW_KEY_Q) > 0)
    {
        moveObj((getDown() * m_engineManager->getDeltaTime()) * 10.f);
    }
}