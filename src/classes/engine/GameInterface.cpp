
#include "GameInterface.h"

#include "../nodeComponents/BasicNode.h"
#include "DebugModel.h"
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
        const std::shared_ptr<DebugModel>& debugModel = SingletonManager::get<DebugModel>();

        do
        {
            double tempTimestamp = glfwGetTime();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            debugModel->setCalculationTimeData(DebugModel::ImGuiNewFrame, glfwGetTime() - tempTimestamp);

            tempTimestamp = glfwGetTime();
            userEventManager->updateEvents(windowManager->getWindow());
            debugModel->setCalculationTimeData(DebugModel::UserEventsUpdate, glfwGetTime() - tempTimestamp);

            tempTimestamp = glfwGetTime();
            engineManager->engineUpdate();
            debugModel->setCalculationTimeData(DebugModel::EngineUpdate, glfwGetTime() - tempTimestamp);

            tempTimestamp = glfwGetTime();
            engineManager->engineDraw();
            debugModel->setCalculationTimeData(DebugModel::EngineDraw, glfwGetTime() - tempTimestamp);

            tempTimestamp = glfwGetTime();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            debugModel->setCalculationTimeData(DebugModel::ImGuiDraw, glfwGetTime() - tempTimestamp);

            tempTimestamp = glfwGetTime();
            glfwSwapBuffers(windowManager->getWindow());
            debugModel->setCalculationTimeData(DebugModel::BufferSwap, glfwGetTime() - tempTimestamp);

            tempTimestamp = glfwGetTime();
            engineManager->engineLateUpdate();
            debugModel->setCalculationTimeData(DebugModel::EngineLateUpdate, glfwGetTime() - tempTimestamp);

            tempTimestamp = glfwGetTime();
            glfwPollEvents();
            debugModel->setCalculationTimeData(DebugModel::glfwPoll, glfwGetTime() - tempTimestamp);

            engineManager->setDeltaTime();
        } while(userEventManager->getUserEvent(GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                glfwWindowShouldClose(windowManager->getWindow()) == 0);

        return 0;
    }
} // namespace Engine