#pragma once

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    class UserEventManager
    {
        public:
            static std::shared_ptr<UserEventManager> getUserEventManager();
            ~UserEventManager() = default;

            using GLFW_KEY = int;
            using GLFW_ACTION = int;

            void clearEvents() { m_userEvents.clear(); };

            void updateEvents(GLFWwindow* window);

            const std::map<GLFW_KEY, GLFW_ACTION>& getUserEvents() const { return m_userEvents; };

            GLFW_ACTION getUserEvent(GLFW_KEY key);

        private:
            UserEventManager() = default;
            static std::shared_ptr<UserEventManager> m_instance;

            std::map<GLFW_KEY, GLFW_ACTION> m_userEvents;
    };
} // namespace Engine
