#pragma once

#include "../uiElements/UiElement.h"
#include "BasicNode.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Engine::Ui
{
    class UiDebugWindow
        : protected UiElement
        , virtual public BasicNode
    {
        public:
            explicit UiDebugWindow(ImGuiWindowFlags flags = ImGuiWindowFlags_None);
            ~UiDebugWindow() = default;

            void drawUi() override;

            std::vector<std::shared_ptr<UiElement>> getContent() const { return m_content; };

            void addContent(const std::shared_ptr<UiElement>& newContent)
            {
                m_content.emplace_back(newContent);
            };

            void removeContent(const std::shared_ptr<UiElement>& content)
            {
                m_content.erase(std::remove(m_content.begin(), m_content.end(), content), m_content.end());
            }

            bool isWindowOpen() const { return m_windowOpen; };

            void setWindowOpen(bool isWindowOpen) { m_windowOpen = isWindowOpen; };

            ImGuiWindowFlags getWindowFlags() const { return m_flags; };

            void addWindowFlag(ImGuiWindowFlags flag) { m_flags = m_flags | flag; };

            void removeWindowFlag(ImGuiWindowFlags flag) { m_flags = m_flags & (~flag); };

            std::string getWindowTitle() const { return m_windowTitle; };

            void setWindowTitle(std::string title) { m_windowTitle = std::move(title); };

        private:
            std::vector<std::shared_ptr<UiElement>> m_content;
            bool m_windowOpen;
            ImGuiWindowFlags m_flags;
            std::string m_windowTitle = "Hello, World!";
    };
} // namespace Engine::Ui
