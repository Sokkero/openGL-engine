#include "CameraComponent.h"

#include "classes/engine/WindowManager.h"
#include "classes/engine/rendering/RenderManager.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace Engine
{
    CameraComponent::CameraComponent()
        : m_projectionMatrix(glm::mat4())
        , m_fov(45.f)
        , m_zNear(.1f)
        , m_zFar(100.f)
        , m_aspectRatio(0.f)
    {
        const auto& windowManager = SingletonManager::get<WindowManager>();

        updateProjectionMatrix(windowManager->getWindowDimensions());
        WindowManager::Callback callback = ([this](GLFWwindow*, int width, int height)
                                            {
                                                updateProjectionMatrix(glm::ivec2(width, height));
                                            });
        WindowManager::AddFramebufferResizeCallback("camera", callback);

    }

    glm::mat4 CameraComponent::getViewMatrix()
    {
        const glm::vec3 pos = getGlobalPosition();
        return glm::lookAt(pos, pos + getForward(), RenderUtils::WORLD_UP);
    }

    void CameraComponent::updateProjectionMatrix(glm::ivec2 windowSize)
    {
        m_aspectRatio = (float)windowSize.x / (float)windowSize.y;
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_zNear, m_zFar);
    }

    void CameraComponent::setFov(float fov)
    {
        m_fov = fov;
        updateProjectionMatrix(SingletonManager::get<WindowManager>()->getWindowDimensions());
    }

    void CameraComponent::setZNear(float zNear)
    {
        m_zNear = zNear;
        updateProjectionMatrix(SingletonManager::get<WindowManager>()->getWindowDimensions());
    }

    void CameraComponent::setZFar(float zFar)
    {
        m_zFar = zFar;
        updateProjectionMatrix(SingletonManager::get<WindowManager>()->getWindowDimensions());
    }
} // namespace Engine