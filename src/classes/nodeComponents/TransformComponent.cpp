
#include "TransformComponent.h"

using namespace Engine;

TransformComponent::TransformComponent()
    : m_modelMatrix(glm::mat4(1.f))
    , m_scale(glm::vec3(1.f))
    , m_position(glm::vec3(0.f))
    , m_rotation(glm::mat4(1.f)) // Quaternions have to be initialized with 1.f!!!
{}

void TransformComponent::moveObj(glm::vec3 dirVec)
{
    m_position += dirVec;
    updateModelMatrix();
};

void TransformComponent::rotateObj(glm::vec3 dirVec, float degrees)
{
    m_rotation *= glm::angleAxis(glm::radians(degrees), glm::normalize(dirVec));
    updateModelMatrix();
};

void TransformComponent::setRotation(glm::vec3 rotDegrees)
{
    glm::vec3 eulerRadians = glm::radians(rotDegrees);
    m_rotation = glm::quat(eulerRadians);
    updateModelMatrix();
};

void TransformComponent::setRotationQuat(glm::quat quat)
{
    m_rotation = quat;
    updateModelMatrix();
};

void TransformComponent::setPosition(glm::vec3 pos)
{
    m_position = pos;
    updateModelMatrix();
};

void TransformComponent::setScale(glm::vec3 scale)
{
    m_scale = scale;
    updateModelMatrix();
};

void TransformComponent::updateModelMatrix()
{
    const glm::mat4 posMat = glm::translate(glm::mat4(1.f), m_position);
    const glm::mat4 rotMat = glm::toMat4(m_rotation);
    const glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), m_scale);

    m_modelMatrix = posMat * rotMat * scaleMat;
}