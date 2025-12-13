#include "BasicNode.h"

#include "../engine/EngineManager.h"
#include "GeometryComponent.h"
#include "UiDebugWindow.h"

#include <iostream>

namespace Engine
{
    unsigned int BasicNode::LASTID = 0;

    BasicNode::BasicNode() : m_parentNode(std::weak_ptr<BasicNode>()) { m_nodeId = getNewUniqueId(); }

    BasicNode::~BasicNode()
    {
        if(!getName().empty())
        {
            std::cout << "Object [" << getName() << "] deconstructed" << std::endl;
        }
    }

    void BasicNode::cleanupNode()
    {
        setParent(nullptr);

        const auto& thisNode = shared_from_this();
        if(const auto geometry = std::dynamic_pointer_cast<GeometryComponent>(thisNode))
        {
            SingletonManager::get<EngineManager>()->removeGeometryFromScene(geometry->getNodeId());
        }
        else if(const auto debugUi = std::dynamic_pointer_cast<Ui::UiDebugWindow>(thisNode))
        {
            SingletonManager::get<EngineManager>()->removeDebugUiFromScene(debugUi->getNodeId());
        }
    }

    std::shared_ptr<BasicNode> BasicNode::getChildNode(int pos) const
    {
        if(pos >= m_childNodes.size())
        {
            return nullptr;
        }
        return m_childNodes[pos];
    }

    void BasicNode::addChild(const std::shared_ptr<BasicNode>& node)
    {
        m_childNodes.emplace_back(node);
        node->setParent(shared_from_this());

        if(auto geometry = std::dynamic_pointer_cast<GeometryComponent>(node))
        {
            SingletonManager::get<EngineManager>()->addGeometryToScene(geometry);
        }
        else if(auto debugUi = std::dynamic_pointer_cast<Ui::UiDebugWindow>(node))
        {
            SingletonManager::get<EngineManager>()->addDebugUiToScene(debugUi);
        }

        node->start();

        if(!node->getName().empty())
        {
            std::cout << "Object [" << node->getName() << "] initialised" << std::endl;
        }
    }

    std::shared_ptr<BasicNode> BasicNode::detatchChild(const std::shared_ptr<BasicNode>& node)
    {
        return detatchChild(node->getNodeId());
    }

    std::shared_ptr<BasicNode> BasicNode::detatchChild(const unsigned int& nodeId)
    {
        for(auto it = m_childNodes.begin(); it != m_childNodes.end();)
        {
            if((*it)->getNodeId() == nodeId)
            {
                auto itNode = *it;
                m_childNodes.erase(it);
                itNode->cleanupNode();
                return itNode;
            }
            ++it;
        }
        return nullptr;
    }

    void BasicNode::deleteChild(const std::shared_ptr<BasicNode>& node) { detatchChild(node->getNodeId()); }

    void BasicNode::deleteChild(const unsigned int& nodeId) { detatchChild(nodeId); }

    std::vector<std::shared_ptr<BasicNode>> BasicNode::detatchAllChildren()
    {
        for(const auto& child : m_childNodes)
        {
            const auto& engineManager = SingletonManager::get<EngineManager>();
            child->callOnAllChildrenRecursiveAndSelf(
                    [engineManager](BasicNode* node) -> void { engineManager->removeGeometryFromScene(node); }
            );
            child->cleanupNode();
            child->setParent(nullptr);
        }

        return std::move(m_childNodes);
    }

    void BasicNode::deleteAllChildren() { detatchAllChildren(); }

    void BasicNode::detatchFromParent()
    {
        const auto& engineManager = SingletonManager::get<EngineManager>();
        callOnAllChildrenRecursiveAndSelf(
                [engineManager](BasicNode* node) -> void { engineManager->removeGeometryFromScene(node); }
        );
        setParent(nullptr);
    }

    void BasicNode::deleteNode() { detatchFromParent(); }

    void BasicNode::callOnAllChildren(const std::function<void(BasicNode*)>& func)
    {
        for(const auto& childNode : m_childNodes)
        {
            func(childNode.get());
        }
    }

    void BasicNode::callOnAllChildrenRecursive(const std::function<void(BasicNode*)>& func)
    {
        for(const auto& childNode : m_childNodes)
        {
            childNode->callOnAllChildrenRecursiveAndSelf(func);
        }
    }

    void BasicNode::callOnAllChildrenAndSelf(const std::function<void(BasicNode*)>& func)
    {
        for(const auto& childNode : m_childNodes)
        {
            func(childNode.get());
        }
        func(this);
    }

    void BasicNode::callOnAllChildrenRecursiveAndSelf(const std::function<void(BasicNode*)>& func)
    {
        for(const auto& childNode : m_childNodes)
        {
            childNode->callOnAllChildrenRecursiveAndSelf(func);
        }
        func(this);
    }

    glm::mat4 BasicNode::getGlobalModelMatrix()
    {
        if(getIsDirty())
        {
            updateGlobalModelMatrix();
        }
        return getCachedGlobalModelMatrix();
    }

    void BasicNode::updateGlobalModelMatrix()
    {
        glm::mat4 matrix = getLocalModelMatrix();
        if(m_parentNode.lock())
        {
            matrix = m_parentNode.lock()->getCachedGlobalModelMatrix() * matrix;
        }

        setCachedGlobalModelMatrix(matrix);
        setIsDirty(false);
    }

    glm::quat BasicNode::getGlobalRotation()
    {
        const glm::mat4 matrix = getGlobalModelMatrix();

        // Extract the 3x3 rotation/scale matrix
        glm::mat3 rotScale = glm::mat3(matrix);

        // Extract scale (length of each basis vector)
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(rotScale[0]));
        scale.y = glm::length(glm::vec3(rotScale[1]));
        scale.z = glm::length(glm::vec3(rotScale[2]));

        // Remove scale from the rotation matrix
        glm::mat3 rotationMatrix;
        rotationMatrix[0] = rotScale[0] / scale.x;
        rotationMatrix[1] = rotScale[1] / scale.y;
        rotationMatrix[2] = rotScale[2] / scale.z;

        // Convert rotation matrix to quaternion
        return glm::quat_cast(rotationMatrix);
    }

    glm::vec3 BasicNode::getGlobalPosition()
    {
        glm::mat4 matrix = getGlobalModelMatrix();
        return matrix[3];
    }

    glm::vec3 BasicNode::getGlobalScale()
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
        return glm::normalize(getGlobalRotation() * glm::vec3(0.f, 0.f, -1.f));
    }

    glm::vec3 BasicNode::getBackwards()
    {
        return glm::normalize(getGlobalRotation() * glm::vec3(0.f, 0.f, 1.f));
    }

    glm::vec3 BasicNode::getLeft()
    {
        return glm::normalize(getGlobalRotation() * glm::vec3(-1.f, 0.f, 0.f));
    }

    glm::vec3 BasicNode::getRight()
    {
        return glm::normalize(getGlobalRotation() * glm::vec3(1.f, 0.f, 0.f));
    }

    glm::vec3 BasicNode::getDown()
    {
        return glm::normalize(getGlobalRotation() * glm::vec3(0.f, -1.f, 0.f));
    }

    glm::vec3 BasicNode::getUp()
    {
        return glm::normalize(getGlobalRotation() * glm::vec3(0.f, 1.f, 0.f));
    }
} // namespace Engine