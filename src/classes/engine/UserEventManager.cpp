
#include "UserEventManager.h"

#include <iostream>
#include <vector>

namespace Engine
{
    UserEventManager::GLFW_ACTION UserEventManager::getUserEvent(GLFW_KEY key)
    {
        if(m_userEvents.find(key) == m_userEvents.end())
        {
            return -1;
        }

        return m_userEvents[key];
    }

    void UserEventManager::updateEvents(GLFWwindow* window)
    {
        std::vector<GLFW_KEY> keysToRemove;
        for(auto& userEvent : m_userEvents)
        {
            if(userEvent.second == GLFW_RELEASE)
            {
                keysToRemove.push_back(userEvent.first);
            }
            else if(glfwGetKey(window, userEvent.first) == GLFW_RELEASE)
            {
                m_userEvents[userEvent.first] = GLFW_RELEASE;
            }
            else if(userEvent.second == GLFW_PRESS && glfwGetKey(window, userEvent.first) == GLFW_PRESS)
            {
                m_userEvents[userEvent.first] = GLFW_REPEAT;
            }
        }

        for(const auto& key : keysToRemove)
        {
            m_userEvents.erase(key);
        }

        for(int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
        {
            if(m_userEvents.find(key) == m_userEvents.end() && glfwGetKey(window, key) != GLFW_RELEASE)
            {
                m_userEvents[key] = glfwGetKey(window, key);
            }
        }

        checkListeners();

        // For debugging purposes
        /*
                for (const auto& userEvent : m_userEvents) {
                    std::cout << "Key " << userEvent.first << ": " << userEvent.second << std::endl;
                }
        */
    }

    void UserEventManager::checkListeners()
    {
        for(const std::pair<int, int> event : m_userEvents)
        {
            for(const auto& listener : m_listeners)
            {
                if(listener.first == event)
                {
                    listener.second();
                }
            }
        }
    }

    glm::vec2 UserEventManager::getWasdInput()
    {
        glm::vec2 input = glm::vec2(0.f, 0.f);

        if(getUserEvent(GLFW_KEY_W) > 0 || getUserEvent(GLFW_KEY_UP) > 0)
        {
            input.y--;
        }
        if(getUserEvent(GLFW_KEY_S) > 0 || getUserEvent(GLFW_KEY_DOWN) > 0)
        {
            input.y++;
        }
        if(getUserEvent(GLFW_KEY_A) > 0 || getUserEvent(GLFW_KEY_LEFT) > 0)
        {
            input.x++;
        }
        if(getUserEvent(GLFW_KEY_D) > 0 || getUserEvent(GLFW_KEY_RIGHT) > 0)
        {
            input.x--;
        }

        return input;
    }
} // namespace Engine