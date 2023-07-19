
#include "UserEventManager.h"

#include <iostream>
#include <vector>

namespace Engine
{
    UserEventManager* UserEventManager::m_instance = nullptr;

    UserEventManager* UserEventManager::getUserEventManager()
    {
        if (m_instance == nullptr)
        {
            m_instance = new UserEventManager();
        }

        return m_instance;
    }

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
        for (auto& userEvent : m_userEvents)
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

        for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
        {
            if (m_userEvents.find(key) == m_userEvents.end() && glfwGetKey(window, key) != GLFW_RELEASE)
            {
                m_userEvents[key] = glfwGetKey(window, key);
            }
        }

        //For debugging purposes
/*
        for (const auto& userEvent : m_userEvents) {
            std::cout << "Key " << userEvent.first << ": " << userEvent.second << std::endl;
        }
*/
    }
}