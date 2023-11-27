#include "UiDebugWindow.h"

using namespace Engine;

UiDebugWindow::UiDebugWindow()
    : m_content(std::vector<std::shared_ptr<UiElement>>())
    , m_windowOpen(true)
    , m_flags(std::vector<ImGuiWindowFlags_>())
{
}

void UiDebugWindow::drawUi()
{
    ImGui::Begin(m_windowTitle.c_str());

    for(const auto& element : m_content)
    {
        element->drawUi();
    }

    ImGui::End();
}
