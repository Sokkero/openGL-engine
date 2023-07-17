#pragma once

#include <map>

#include <GLFW/glfw3.h>

namespace Engine
{
    class UserEventManager
    {
        public:
            static UserEventManager* getUserEventManager();
            ~UserEventManager() = default;

            using GLFW_KEY = int;
            using GLFW_ACTION = int;

            void clearEvents() { m_userEvents.clear(); };
            void updateEvents(GLFWwindow* window);

            const std::map<GLFW_KEY, GLFW_ACTION>& getUserEvents() const { return m_userEvents; };
            GLFW_ACTION getUserEvent(GLFW_KEY key);

        private:
            UserEventManager() = default;
            static UserEventManager* m_instance;

            std::map<GLFW_KEY, GLFW_ACTION> m_userEvents;
    };
}
