
#include "CameraModel.h"

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