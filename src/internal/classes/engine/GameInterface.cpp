#include "GameInterface.h"

#include "classes/engine/EngineManager.h"
#include "classes/engine/PerformanceModel.h"
#include "classes/engine/UserEventManager.h"
#include "classes/engine/WindowManager.h"
#include "classes/nodeComponents/BasicNode.h"
#include "utils/TimeUtils.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Engine
{
    GameInterface::GameInterface()
    {
        SingletonManager::get<WindowManager>()->startWindow();
    }

    int GameInterface::startGame()
    {
        const std::shared_ptr<EngineManager>& engineManager = SingletonManager::get<EngineManager>();
        if(!engineManager->engineStart())
        {
            return 1;
        }

        const std::shared_ptr<UserEventManager>& userEventManager = SingletonManager::get<UserEventManager>();
        const std::shared_ptr<WindowManager>& windowManager = SingletonManager::get<WindowManager>();
        const std::shared_ptr<PerformanceModel>& debugModel = SingletonManager::get<PerformanceModel>();

        do
        {
            TimeUtils::SysTimestamp tempTimestamp = TimeUtils::GetSystemsTimestamp();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            debugModel->setEngineEventTimeData(DebugUtils::ImGuiNewFrame, TimeUtils::GetDurationSince(tempTimestamp));

            tempTimestamp = TimeUtils::GetSystemsTimestamp();
            userEventManager->updateEvents(windowManager->getWindow());
            debugModel->setEngineEventTimeData(DebugUtils::UserEventsUpdate, TimeUtils::GetDurationSince(tempTimestamp));

            tempTimestamp = TimeUtils::GetSystemsTimestamp();
            engineManager->engineUpdate();
            debugModel->setEngineEventTimeData(DebugUtils::EngineUpdate, TimeUtils::GetDurationSince(tempTimestamp));

            tempTimestamp = TimeUtils::GetSystemsTimestamp();
            engineManager->engineDraw();
            debugModel->setEngineEventTimeData(DebugUtils::EngineDraw, TimeUtils::GetDurationSince(tempTimestamp));

            tempTimestamp = TimeUtils::GetSystemsTimestamp();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            debugModel->setEngineEventTimeData(DebugUtils::ImGuiDraw, TimeUtils::GetDurationSince(tempTimestamp));

            tempTimestamp = TimeUtils::GetSystemsTimestamp();
            glfwSwapBuffers(windowManager->getWindow());
            debugModel->setEngineEventTimeData(DebugUtils::BufferSwap, TimeUtils::GetDurationSince(tempTimestamp));

            tempTimestamp = TimeUtils::GetSystemsTimestamp();
            engineManager->engineLateUpdate();
            debugModel->setEngineEventTimeData(DebugUtils::EngineLateUpdate, TimeUtils::GetDurationSince(tempTimestamp));

            tempTimestamp = TimeUtils::GetSystemsTimestamp();
            glfwPollEvents();
            debugModel->setEngineEventTimeData(DebugUtils::glfwPoll, TimeUtils::GetDurationSince(tempTimestamp));

            engineManager->setDeltaTime();
        } while(userEventManager->getUserEvent(GLFW_KEY_ESCAPE) != GLFW_PRESS
                && glfwWindowShouldClose(windowManager->getWindow()) == 0);

        return 0;
    }
} // namespace Engine