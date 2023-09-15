
#include "GameInterface.h"

#include "../NodeComponents/BasicNode.h"

namespace Engine
{
    GameInterface::GameInterface()
        : m_engineManager(nullptr)
        , m_windowManager(nullptr)
        , m_userEventManager(nullptr)
    {
        m_windowManager = std::make_shared<WindowManager>();
        m_windowManager->startWindow();

        m_engineManager = std::make_shared<EngineManager>();
        m_userEventManager = std::make_shared<UserEventManager>();

        BasicNode::setWindowManager(m_windowManager);
        BasicNode::setEngineManager(m_engineManager);
        BasicNode::setUserEventManager(m_userEventManager);
    }

    int GameInterface::startGame()
    {
        if(!m_engineManager->engineStart())
        {
            return 1;
        }

        do
        {
            m_userEventManager->updateEvents(m_windowManager->getWindow());
            m_engineManager->engineUpdate();
            m_engineManager->engineDraw();

            glfwSwapBuffers(m_windowManager->getWindow());
            glfwPollEvents();

            m_engineManager->setDeltaTime();
        } while(m_userEventManager->getUserEvent(GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                glfwWindowShouldClose(m_windowManager->getWindow()) == 0);

        return 0;
    }
} // namespace Engine