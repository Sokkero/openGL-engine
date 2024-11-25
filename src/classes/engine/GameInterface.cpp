
#include "GameInterface.h"

#include "../nodeComponents/BasicNode.h"
#include "EngineManager.h"
#include "UserEventManager.h"
#include "WindowEventCallbackHelper.h"
#include "WindowManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Engine
{
    GameInterface::GameInterface() { SingletonManager::get<WindowManager>()->startWindow(); }

    int GameInterface::startGame()
    {
        const std::shared_ptr<EngineManager>& engineManager = SingletonManager::get<EngineManager>();
        if(!engineManager->engineStart())
        {
            return 1;
        }

        const std::shared_ptr<UserEventManager>& userEventManager = SingletonManager::get<UserEventManager>();
        const std::shared_ptr<WindowManager>& windowManager = SingletonManager::get<WindowManager>();

        do
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            userEventManager->updateEvents(windowManager->getWindow());
            engineManager->engineUpdate();

            engineManager->engineDraw();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(windowManager->getWindow());

            engineManager->engineLateUpdate();

            glfwPollEvents();

            engineManager->setDeltaTime();
        } while(userEventManager->getUserEvent(GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                glfwWindowShouldClose(windowManager->getWindow()) == 0);

        return 0;
    }
} // namespace Engine