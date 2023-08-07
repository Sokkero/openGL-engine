#include "CameraActor.h"

#include "../engine/EngineManager.h"
#include "../engine/UserEventManager.h"
#include "../engine/WindowManager.h"

#include <iostream>

void CameraActor::start() {}

void CameraActor::update()
{
    if(getUserEventManager()->getUserEvent(GLFW_KEY_W) > 0)
    {
        getParentNode()->rotateObj(getParentNode()->getLeft(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getForward() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(getUserEventManager()->getUserEvent(GLFW_KEY_S) > 0)
    {
        getParentNode()->rotateObj(getParentNode()->getRight(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getBackwards() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(getUserEventManager()->getUserEvent(GLFW_KEY_A) > 0)
    {
        getParentNode()->rotateObj(getParentNode()->getDown(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getLeft() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(getUserEventManager()->getUserEvent(GLFW_KEY_D) > 0)
    {
        getParentNode()->rotateObj(getParentNode()->getUp(), getEngineManager()->getDeltaTime() * 100);
        // moveObj((getRight() * m_engineManager->getDeltaTime()) * 10.f);
    }
    else if(getUserEventManager()->getUserEvent(GLFW_KEY_E) > 0)
    {
        moveObj((getForward() * getEngineManager()->getDeltaTime()) * 10.f);
    }
    else if(getUserEventManager()->getUserEvent(GLFW_KEY_Q) > 0)
    {
        moveObj((getBackwards() * getEngineManager()->getDeltaTime()) * 10.f);
    }
}