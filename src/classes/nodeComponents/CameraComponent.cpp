#include "CameraComponent.h"

#include "../engine/WindowManager.h"
#include "../engine/rendering/RenderManager.h"

#include "glm/ext/matrix_clip_space.hpp"

namespace Engine
{
    CameraComponent::CameraComponent()
        : m_projectionMatrix(glm::mat4())
        , m_fov(45.f)
        , m_zNear(.1f)
        , m_zFar(100.f)
    {
        const glm::vec2 dim = getWindowManager()->getWindowDimensions();
        m_aspectRatio = dim.x / dim.y;

        updateProjectionMatrix();
    }

    glm::mat4 CameraComponent::getViewMatrix()
    {
        const glm::vec3 globalModel = getGlobalModelMatrix()[2];
        return glm::lookAt(getGlobalPosition(), getForward() + globalModel, WORLD_UP);
    }

    void CameraComponent::updateProjectionMatrix()
    {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_zNear, m_zFar);
    }

    void CameraComponent::setFov(float fov)
    {
        m_fov = fov;
        updateProjectionMatrix();
    }

    void CameraComponent::setAspectRatio(float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        updateProjectionMatrix();
    }

    void CameraComponent::setZNear(float zNear)
    {
        m_zNear = zNear;
        updateProjectionMatrix();
    }

    void CameraComponent::setZFar(float zFar)
    {
        m_zFar = zFar;
        updateProjectionMatrix();
    }
} // namespace Engine