
#include "TransformComponent.h"

using namespace Engine;

TransformComponent::TransformComponent()
    : m_globalModelMatrix(glm::mat4(1.f))
    , m_scale(glm::vec3(1.f))
    , m_position(glm::vec3(0.f))
    , m_rotation(glm::mat4(1.f)) // Quaternions have to be initialized with 1.f!!!
    , m_isDirty(true)
{
}

void TransformComponent::moveObj(glm::vec3 dirVec)
{
    m_position += dirVec;
    m_isDirty = true;
};

void TransformComponent::rotateObj(glm::vec3 dirVec, float degrees)
{
    m_rotation *= glm::angleAxis(glm::radians(degrees), glm::normalize(dirVec));
    m_isDirty = true;
};

void TransformComponent::setRotation(glm::vec3 rotDegrees)
{
    glm::vec3 eulerRadians = glm::radians(rotDegrees);
    m_rotation = glm::quat(eulerRadians);
    m_isDirty = true;
};

void TransformComponent::setRotationQuat(glm::quat quat)
{
    m_rotation = quat;
    m_isDirty = true;
};

void TransformComponent::setPosition(glm::vec3 pos)
{
    m_position = pos;
    m_isDirty = true;
};

void TransformComponent::setScale(glm::vec3 scale)
{
    m_scale = scale;
    m_isDirty = true;
};

glm::mat4 TransformComponent::getLocalModelMatrix() const
{
    const glm::mat4 posMat = glm::translate(glm::mat4(1.f), m_position);
    const glm::mat4 rotMat = glm::toMat4(m_rotation);
    const glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), m_scale);

    return posMat * rotMat * scaleMat;
}