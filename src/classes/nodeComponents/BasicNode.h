#pragma once

#include "TransformComponent.h"

#include <string>
#include <utility>
#include <vector>

namespace Engine
{
    class WindowManager;
    class EngineManager;
    class UserEventManager;

    class BasicNode : virtual public TransformComponent
    {
        public:
            BasicNode();
            virtual ~BasicNode() = default;

            virtual void start() {};
            virtual void update() {};

            void setName(std::string name) { m_name = std::move(name); };

            std::string getName() const { return m_name; };

            void addChild(const std::shared_ptr<BasicNode>& node);

            void setParent(BasicNode* node) { m_parentNode = node; };

            BasicNode* getParentNode() const { return m_parentNode; };

            std::shared_ptr<BasicNode> getChildNode(int pos) const;

            std::vector<std::shared_ptr<BasicNode>> getChildNodes() const { return m_childNodes; };

            int getChildCount() const { return int(m_childNodes.size()); };

            std::vector<std::shared_ptr<BasicNode>> removeAllChildNodes();
            void deleteAllChildNodes();
            void callOnAllChildren(const std::function<void(BasicNode*)>& func);
            void callOnAllChildrenRecursive(const std::function<void(BasicNode*)>& func);
            void callOnAllChildrenAndSelf(const std::function<void(BasicNode*)>& func);
            void callOnAllChildrenRecursiveAndSelf(const std::function<void(BasicNode*)>& func);

            glm::mat4 getGlobalModelMatrix() const;
            glm::quat getGlobalRotation() const;
            glm::vec3 getGlobalPosition() const;
            glm::vec3 getGlobalScale() const;

            static void setWindowManager(std::shared_ptr<WindowManager> newWindowManager)
            {
                WINDOW_MANAGER = std::move(newWindowManager);
            };

            static std::shared_ptr<WindowManager> getWindowManager() { return WINDOW_MANAGER; };

            static void setEngineManager(std::shared_ptr<EngineManager> newEngineManager)
            {
                ENGINE_MANAGER = std::move(newEngineManager);
            };

            static std::shared_ptr<EngineManager> getEngineManager() { return ENGINE_MANAGER; };

            static void setUserEventManager(std::shared_ptr<UserEventManager> newUserEventManager)
            {
                USER_EVENT_MANAGER = std::move(newUserEventManager);
            };

            static std::shared_ptr<UserEventManager> getUserEventManager() { return USER_EVENT_MANAGER; };

            glm::vec3 getForward() const;

            glm::vec3 getBackwards() const;

            glm::vec3 getLeft() const;

            glm::vec3 getRight() const;

            glm::vec3 getDown() const;

            glm::vec3 getUp() const;

        private:
            std::string m_name;
            BasicNode* m_parentNode;
            std::vector<std::shared_ptr<BasicNode>> m_childNodes;

            static std::shared_ptr<WindowManager> WINDOW_MANAGER;
            static std::shared_ptr<EngineManager> ENGINE_MANAGER;
            static std::shared_ptr<UserEventManager> USER_EVENT_MANAGER;
    };
} // namespace Engine