#include "BasicNode.h"

#include "../WindowManager.h"

namespace Engine
{
    WindowManager* BasicNode::windowManager = nullptr;

    std::shared_ptr<BasicNode> BasicNode::getChildNode(int pos) const
    {
        if(pos >= m_childNodes.size())
        {
            return {nullptr};
        }
        return m_childNodes[pos];
    }

    void BasicNode::addAsChild(std::shared_ptr<BasicNode> node)
    {
        m_childNodes.emplace_back(node);
        node->setParent(std::make_shared<BasicNode>(*this));

        node->start();
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
}