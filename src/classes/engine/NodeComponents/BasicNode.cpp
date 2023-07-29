#include "BasicNode.h"

#include "../WindowManager.h"

#include <iostream>

namespace Engine
{
    WindowManager* BasicNode::windowManager = nullptr;

    std::shared_ptr<BasicNode> BasicNode::getChildNode(int pos) const
    {
        if(pos >= m_childNodes.size())
        {
            return nullptr;
        }
        return m_childNodes[pos];
    }

    void BasicNode::addAsChild(std::shared_ptr<BasicNode> node)
    {
        m_childNodes.emplace_back(node);
        node->setParent(std::make_shared<BasicNode>(*this));

        node->start();
        std::cout << "Object [" << node->getName() <<"] initialised" << std::endl;
    }

    void BasicNode::removeFromParent(std::shared_ptr<BasicNode> node)
    {
        m_childNodes.erase(std::remove(m_childNodes.begin(), m_childNodes.end(), node), m_childNodes.end());
        node->setParent(nullptr);
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
            return getLocalModelMatrix() * m_parentNode->getLocalModelMatrix();
        }
        return getLocalModelMatrix();
    }

    glm::vec4 BasicNode::getGlobalPosition() const
    {
        return getGlobalModelMatrix()[3];
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
}