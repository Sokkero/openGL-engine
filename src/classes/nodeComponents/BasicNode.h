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

    /**
     * @brief The BasicNode class represents a basic node in the engine's scene graph.
     *
     * This class is a subclass of TransformComponent and provides basic functionality for managing a node's hierarchy,
     * including parent-child relationships, adding and removing child nodes, and accessing global transformation properties.
     */
    class BasicNode : virtual public TransformComponent, public std::enable_shared_from_this<BasicNode>
    {
        public:
            BasicNode();
            virtual ~BasicNode() = default;

            /**
             * @brief Called right before the node gets added to the scene.
             *
             * This function can be overridden by derived classes to perform any necessary operations before the node is added to the scene.
             */
            virtual void awake() {};

            /**
             * @brief Called when the node is started.
             *
             * This function can be overridden by derived classes to perform any necessary initialization when the node is started.
             */
            virtual void start() {};

            /**
             * @brief Called every frame to update the node.
             *
             * This function can be overridden by derived classes to update the node's state every frame.
             */
            virtual void update() {};

            /**
             * @brief Sets the name of the node.
             *
             * @param name The name of the node.
             */
            void setName(std::string name) { m_name = std::move(name); };

            /**
             * @brief Gets the name of the node.
             *
             * @return The name of the node.
             */
            std::string getName() const { return m_name; };

            /**
             * @brief Sets the parent node of this node.
             *
             * @param node The parent node to set.
             */
            void setParent(std::shared_ptr<BasicNode> node) { m_parentNode = node; };

            /**
             * @brief Adds a child node to this node.
             *
             * @param node The child node to add.
             */
            void addChild(const std::shared_ptr<BasicNode>& node);

            /**
             * @brief Detatches a child node from this node.
             *
             * @param node The child node to be detatched.
             */
            std::shared_ptr<BasicNode> detatchChild(const std::shared_ptr<BasicNode>& node);

            /**
             * @brief Detatches a child node from this node.
             *
             * @param uint The child ID of the node to be detatched.
             */
            std::shared_ptr<BasicNode> detatchChild(const unsigned int& nodeId);

            /**
             * @brief Deletes a child node of this node.
             */
            void deleteChild(const std::shared_ptr<BasicNode>& node);

            /**
             * @brief Detatches a child node of this node.
             */
            void deleteChild(const unsigned int& nodeId);

            /**
             * @brief Detatches all children of this node.
             *
             * @return A vector containing all the child nodes that were detatched.
             */
            std::vector<std::shared_ptr<BasicNode>> detatchAllChildren();

            /**
             * @brief Deletes all the child nodes of this node.
             */
            void deleteAllChildren();

            /**
             * @brief Detatches this node from its parents.
             */
            void detatchFromParent();

            /**
             * @brief Deletes this node and all its children.
             */
            void deleteNode();

            /**
             * @brief Gets the parent node of this node.
             *
             * @return The parent node of this node.
             */
            std::shared_ptr<BasicNode> getParentNode() const { return m_parentNode.lock(); };

            /**
             * @brief Gets the child node at the specified position.
             *
             * @param pos The position of the child node.
             * @return The child node at the specified position.
             */
            std::shared_ptr<BasicNode> getChildNode(int pos) const;

            /**
             * @brief Gets all the child nodes of this node.
             *
             * @return A vector containing all the child nodes of this node.
             */
            std::vector<std::shared_ptr<BasicNode>> getChildNodes() const { return m_childNodes; };

            /**
             * @brief Gets the number of child nodes of this node.
             *
             * @return The number of child nodes of this node.
             */
            int getChildCount() const { return int(m_childNodes.size()); };

            /**
             * @brief Calls a function on all the children of this node.
             *
             * @param func The function to call on each child node.
             */
            void callOnAllChildren(const std::function<void(BasicNode*)>& func);

            /**
             * @brief Calls a function on all the children of this node recursively.
             *
             * @param func The function to call on each child node.
             */
            void callOnAllChildrenRecursive(const std::function<void(BasicNode*)>& func);

            /**
             * @brief Calls a function on all the children of this node and on the node itself.
             *
             * @param func The function to call on each child node and on the node itself.
             */
            void callOnAllChildrenAndSelf(const std::function<void(BasicNode*)>& func);

            /**
             * @brief Calls a function on all the children of this node recursively and then on the node itself.
             *
             * @param func The function to call on each child node and on the node itself.
             */
            void callOnAllChildrenRecursiveAndSelf(const std::function<void(BasicNode*)>& func);

            /**
             * @brief Gets the global model matrix of this node.
             *
             * @return The global model matrix of this node.
             */
            glm::mat4 getGlobalModelMatrix() const;

            /**
             * @brief Gets the global rotation of this node.
             *
             * @return The global rotation of this node.
             */
            glm::quat getGlobalRotation() const;

            /**
             * @brief Gets the global position of this node.
             *
             * @return The global position of this node.
             */
            glm::vec3 getGlobalPosition() const;

            /**
             * @brief Gets the global scale of this node.
             *
             * @return The global scale of this node.
             */
            glm::vec3 getGlobalScale() const;

            /**
             * @brief Sets the window manager for the engine.
             *
             * @param newWindowManager The new window manager to set.
             */
            static void setWindowManager(std::shared_ptr<WindowManager> newWindowManager)
            {
                WINDOW_MANAGER = std::move(newWindowManager);
            };

            /**
             * @brief Gets the window manager for the engine.
             *
             * @return The window manager for the engine.
             */
            static std::shared_ptr<WindowManager> getWindowManager() { return WINDOW_MANAGER; };

            /**
             * @brief Sets the engine manager for the engine.
             *
             * @param newEngineManager The new engine manager to set.
             */
            static void setEngineManager(std::shared_ptr<EngineManager> newEngineManager)
            {
                ENGINE_MANAGER = std::move(newEngineManager);
            };

            /**
             * @brief Gets the engine manager for the engine.
             *
             * @return The engine manager for the engine.
             */
            static std::shared_ptr<EngineManager> getEngineManager() { return ENGINE_MANAGER; };

            /**
             * @brief Sets the user event manager for the engine.
             *
             * @param newUserEventManager The new user event manager to set.
             */
            static void setUserEventManager(std::shared_ptr<UserEventManager> newUserEventManager)
            {
                USER_EVENT_MANAGER = std::move(newUserEventManager);
            };

            /**
             * @brief Gets the user event manager for the engine.
             *
             * @return The user event manager for the engine.
             */
            static std::shared_ptr<UserEventManager> getUserEventManager() { return USER_EVENT_MANAGER; };

            /**
             * @brief Gets the forward direction of this node.
             *
             * @return The forward direction of this node.
             */
            glm::vec3 getForward() const;

            /**
             * @brief Gets the backwards direction of this node.
             *
             * @return The backwards direction of this node.
             */
            glm::vec3 getBackwards() const;

            /**
             * @brief Gets the left direction of this node.
             *
             * @return The left direction of this node.
             */
            glm::vec3 getLeft() const;

            /**
             * @brief Gets the right direction of this node.
             *
             * @return The right direction of this node.
             */
            glm::vec3 getRight() const;

            /**
             * @brief Gets the down direction of this node.
             *
             * @return The down direction of this node.
             */
            glm::vec3 getDown() const;

            /**
             * @brief Gets the up direction of this node.
             *
             * @return The up direction of this node.
             */
            glm::vec3 getUp() const;

            /**
             * @brief Gets a pointer to the component T.
             *
             * @return The requested component
             */
            template<typename T>
            T* getComponent()
            {
                return dynamic_cast<T*>(this);
            };

            /**
             * @brief Returns the unique ID of the node.
             * @return An unsigned int, the unique ID of the node
             */
            unsigned int getNodeId() const { return m_nodeId; }

        private:
            std::string m_name;
            std::weak_ptr<BasicNode> m_parentNode;
            std::vector<std::shared_ptr<BasicNode>> m_childNodes;
            unsigned int m_nodeId;

            static unsigned int LASTID;

            static unsigned int getNewUniqueId()
            {
                return ++LASTID;
            }

            static std::shared_ptr<WindowManager> WINDOW_MANAGER;
            static std::shared_ptr<EngineManager> ENGINE_MANAGER;
            static std::shared_ptr<UserEventManager> USER_EVENT_MANAGER;
    };
} // namespace Engine