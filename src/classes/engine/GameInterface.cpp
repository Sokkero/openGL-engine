
#include "GameInterface.h"

#include "../nodeComponents/BasicNode.h"
#include "WindowEventCallbackHelper.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

        WindowEventCallbackHelper::ENIGNE_MANAGER = m_engineManager;
    }

    int GameInterface::startGame()
    {
        if(!m_engineManager->engineStart())
        {
            return 1;
        }

        do
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            m_userEventManager->updateEvents(m_windowManager->getWindow());
            m_engineManager->engineUpdate();

            m_engineManager->engineDraw();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_windowManager->getWindow());

            m_engineManager->engineLateUpdate();

            glfwPollEvents();

            m_engineManager->setDeltaTime();
        } while(m_userEventManager->getUserEvent(GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                glfwWindowShouldClose(m_windowManager->getWindow()) == 0);

        return 0;
    }
} // namespace Engine