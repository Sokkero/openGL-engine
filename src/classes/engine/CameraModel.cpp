
#include "CameraModel.h"

#include "WindowManager.h"
#include "UserEventManager.h"
#include "EngineManager.h"

#include <iostream>

namespace Engine
{
    CameraModel::CameraModel()
    : m_projectionMatrix(glm::mat4())
    , m_fov (45.f)
    , m_aspectRatio((float)1024 / (float)640)
    , m_zNear(.1f)
    , m_zFar(100.f)
    {
        m_projectionMatrix = glm::perspective(
                glm::radians(m_fov),
                m_aspectRatio,
                m_zNear,
                m_zFar
        );
    }

    void CameraModel::update()
    {
        if(getWindowManager()->getUserEventManager()->getUserEvent(GLFW_KEY_W) > 0)
        {
            moveObj((getForward() * getWindowManager()->getEngine()->getDeltaTime()) * 500.f);
        }
        else if(getWindowManager()->getUserEventManager()->getUserEvent(GLFW_KEY_S) > 0)
        {
            moveObj((getBackwards() * getWindowManager()->getEngine()->getDeltaTime()) * 500.f);
        }
    }

    void CameraModel::updateProjectionMatrix()
    {
        m_projectionMatrix = glm::perspective(
                glm::radians(m_fov),
                m_aspectRatio,
                m_zNear,
                m_zFar
        );
    }

    void CameraModel::setFov(float fov)
    {
        m_fov = fov;
        updateProjectionMatrix();
    }

    void CameraModel::setAspectRatio(float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        updateProjectionMatrix();
    }

    void CameraModel::setZNear(float zNear)
    {
        m_zNear = zNear;
        updateProjectionMatrix();
    }

    void CameraModel::setZFar(float zFar)
    {
        m_zFar = zFar;
        updateProjectionMatrix();
    }
}