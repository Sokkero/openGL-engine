#include "UiDebugWindow.h"

using namespace Engine::Ui;

UiDebugWindow::UiDebugWindow(ImGuiWindowFlags flags)
    : m_content(std::vector<std::shared_ptr<UiElement>>())
    , m_windowOpen(true)
    , m_flags(flags)
{
    setName(m_windowTitle);
}

void UiDebugWindow::drawUi()
{
    if(!m_windowOpen && m_windowIsClosable)
    {
        return;
    }

    ImGui::Begin(m_windowTitle.c_str(), m_windowIsClosable ? &m_windowOpen : nullptr, m_flags);

    for(const auto& element : m_content)
    {
        element->drawUi();
    }

    ImGui::End();
}
