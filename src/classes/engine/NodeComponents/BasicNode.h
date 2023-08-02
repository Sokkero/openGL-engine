#pragma once

#include "TransformComponent.h"

#include <string>
#include <vector>

namespace Engine
{
    class WindowManager;
    class EngineManager;
    class UserEventManager;

    class BasicNode : virtual public TransformComponent
    {
        public:
            BasicNode() = default;
            virtual ~BasicNode() = default;

            virtual void start() {};
            virtual void update() {};

            void setName(std::string name) { m_name = name; };

            std::string getName() const { return m_name; };

            void addAsChild(std::shared_ptr<BasicNode> node);
            void removeFromParent(std::shared_ptr<BasicNode> node);

            void setParent(std::shared_ptr<BasicNode> node) { m_parentNode = node; };

            std::shared_ptr<BasicNode> getParentNode() const { return m_parentNode; };

            std::shared_ptr<BasicNode> getChildNode(int pos) const;

            std::vector<std::shared_ptr<BasicNode>> getChildNodes() const { return m_childNodes; };

            int getChildCount() const { return int(m_childNodes.size()); };

            std::vector<std::shared_ptr<BasicNode>> removeAllChildNodes();
            void deleteAllChildNodes();
            void callOnAllChildren(const std::function<void(BasicNode*)>& func);

            glm::mat4 getGlobalModelMatrix() const;
            glm::vec4 getGlobalPosition() const;
            glm::vec3 getGlobalScale() const;

            static void setWindowManager(std::shared_ptr<WindowManager> newWindowManager){WINDOW_MANAGER = newWindowManager;};
            static std::shared_ptr<WindowManager> getWindowManager() { return WINDOW_MANAGER; };

            static void setEngineManager(std::shared_ptr<EngineManager> newEngineManager){ENGINE_MANAGER = newEngineManager;};
            static std::shared_ptr<EngineManager> getEngineManager() { return ENGINE_MANAGER; };

            static void setUserEventManager(std::shared_ptr<UserEventManager> newUserEventManager){USER_EVENT_MANAGER = newUserEventManager;};
            static std::shared_ptr<UserEventManager> getUserEventManager() { return USER_EVENT_MANAGER; };

        private:
            std::string m_name;
            std::shared_ptr<BasicNode> m_parentNode;
            std::vector<std::shared_ptr<BasicNode>> m_childNodes;

            static std::shared_ptr<WindowManager> WINDOW_MANAGER;
            static std::shared_ptr<EngineManager> ENGINE_MANAGER;
            static std::shared_ptr<UserEventManager> USER_EVENT_MANAGER;
    };
} // namespace Engine