#include "CameraActor.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"

#include <iostream>

void CameraActor::start() {}

void CameraActor::update()
{
    if(getUserEventManager()->getUserEvent(GLFW_KEY_W) > 0)
    {
        getParentNode()->rotateObj(getRight(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getForward() * m_engineManager->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_S) > 0)
    {
        getParentNode()->rotateObj(getLeft(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getBackwards() * m_engineManager->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_A) > 0)
    {
        getParentNode()->rotateObj(getUp(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getLeft() * m_engineManager->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_D) > 0)
    {
        getParentNode()->rotateObj(getDown(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getRight() * m_engineManager->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_E) > 0)
    {
        moveObj((getForward() * getEngineManager()->getDeltaTime()) * 10.f);
    }
    if(getUserEventManager()->getUserEvent(GLFW_KEY_Q) > 0)
    {
        moveObj((getBackwards() * getEngineManager()->getDeltaTime()) * 10.f);
    }
}