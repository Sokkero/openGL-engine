#include "BasicNode.h"

#include <iostream>

namespace Engine
{
    std::shared_ptr<WindowManager> BasicNode::WINDOW_MANAGER = nullptr;
    std::shared_ptr<EngineManager> BasicNode::ENGINE_MANAGER = nullptr;
    std::shared_ptr<UserEventManager> BasicNode::USER_EVENT_MANAGER = nullptr;

    std::shared_ptr<BasicNode> BasicNode::getChildNode(int pos) const
    {
        if(pos >= m_childNodes.size())
        {
            return nullptr;
        }
        return m_childNodes[pos];
    }

    void BasicNode::addChild(std::shared_ptr<BasicNode> node)
    {
        m_childNodes.emplace_back(node);
        node->setParent(this);

        node->start();
        std::cout << "Object [" << node->getName() << "] initialised" << std::endl;
    }

    std::vector<std::shared_ptr<BasicNode>> BasicNode::removeAllChildNodes()
    {
        return std::move(m_childNodes);
    }

    void BasicNode::deleteAllChildNodes()
    {
        for(const auto& childNode : m_childNodes)
        {
            childNode->deleteAllChildNodes();
        }
        m_childNodes.clear();
    }

    void BasicNode::callOnAllChildren(const std::function<void(BasicNode*)>& func)
    {
        for(const auto& childNode : m_childNodes)
        {
            childNode->callOnAllChildren(func);
        }
        func(this);
    }

    glm::mat4 BasicNode::getGlobalModelMatrix() const
    {
        if(m_parentNode)
        {
            return m_parentNode->getGlobalModelMatrix() * getModelMatrix();
        }
        return getModelMatrix();
    }

    glm::vec3 BasicNode::getGlobalPosition() const
    {
        glm::mat4 matrix = getGlobalModelMatrix();
        return matrix[3];
    }

    glm::vec3 BasicNode::getGlobalScale() const
    {
        glm::vec3 scale;
        const auto& matrix = getGlobalModelMatrix();
        scale.x = glm::length(glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]));
        scale.y = glm::length(glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]));
        scale.z = glm::length(glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]));
        return scale;
    }

    glm::vec3 BasicNode::getForward()
    {
        return glm::normalize(glm::rotate(getRotationQuat(), glm::vec3(0.0f, 0.0f, -1.0f)));
    };

    glm::vec3 BasicNode::getBackwards()
    {
        return glm::normalize(glm::rotate(getRotationQuat(), glm::vec3(0.0f, 0.0f, 1.0f)));
    };

    glm::vec3 BasicNode::getLeft()
    {
        return glm::normalize(glm::rotate(getRotationQuat(), glm::vec3(-1.0f, 0.0f, 0.0f)));
    };

    glm::vec3 BasicNode::getRight()
    {
        return glm::normalize(glm::rotate(getRotationQuat(), glm::vec3(1.0f, 0.0f, 0.0f)));
    };

    glm::vec3 BasicNode::getDown()
    {
        return glm::normalize(glm::rotate(getRotationQuat(), glm::vec3(0.0f, -1.0f, 0.0f)));
    };

    glm::vec3 BasicNode::getUp()
    {
        return glm::normalize(glm::rotate(getRotationQuat(), glm::vec3(0.0f, 1.0f, 0.0f)));
    };
} // namespace Engine