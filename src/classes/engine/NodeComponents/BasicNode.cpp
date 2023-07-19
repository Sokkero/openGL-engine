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
    }

    void BasicNode::removeFromParent(std::shared_ptr<BasicNode> node)
    {
        m_childNodes.erase(std::remove(m_childNodes.begin(), m_childNodes.end(), node), m_childNodes.end());
        node->setParent(nullptr);
    }

    void BasicNode::removeAllChildNodes()
    {
        for(const auto& childNode : m_childNodes)
        {
            childNode->removeAllChildNodes();
        }
        m_childNodes.clear();
    }

    void BasicNode::callOnAllNodes(const std::function<void(BasicNode*)>& func)
    {
        for(const auto& childNode : m_childNodes)
        {
            childNode->callOnAllNodes(func);
        }
        func(this);
    }
}