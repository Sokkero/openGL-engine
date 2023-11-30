#pragma once

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace Engine
{
    class UserEventManager
    {
        public:
            using Callback = std::function<void()>;

            UserEventManager() = default;
            ~UserEventManager() = default;

            using GLFW_KEY = int;
            using GLFW_ACTION = int;

            void clearEvents() { m_userEvents.clear(); };

            void updateEvents(GLFWwindow* window);

            void checkListeners();

            void addListener(std::pair<GLFW_KEY, GLFW_ACTION> action, Callback listener)
            {
                m_listeners.emplace_back(std::pair<std::pair<GLFW_KEY, GLFW_ACTION>, Callback>(action, listener));
            };

            void clearListener() { m_listeners.clear(); };

            const std::map<GLFW_KEY, GLFW_ACTION>& getUserEvents() const { return m_userEvents; };

            GLFW_ACTION getUserEvent(GLFW_KEY key);

            glm::vec2 getWasdInput();

        private:
            std::map<GLFW_KEY, GLFW_ACTION> m_userEvents;
            std::vector<std::pair<std::pair<GLFW_KEY, GLFW_ACTION>, Callback>> m_listeners;
    };
} // namespace Engine
