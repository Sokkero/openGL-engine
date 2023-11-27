#pragma once

#include "../uiElements/UiElement.h"
#include "BasicNode.h"
#include <memory>
#include <string>
#include <vector>

namespace Engine
{
    class UiDebugWindow : protected UiElement, virtual public BasicNode
    {
        public:
            UiDebugWindow();
            ~UiDebugWindow() = default;

            void drawUi() override;

            std::vector<std::shared_ptr<UiElement>> getContent() const { return m_content; };

            void addContent(std::shared_ptr<UiElement> newContent) { m_content.emplace_back(newContent); };

            void removeContent(std::shared_ptr<UiElement> content)
            {
                m_content.erase(std::remove(m_content.begin(), m_content.end(), content), m_content.end());
            }

            bool isWindowOpen() const { return m_windowOpen; };

            void setWindowOpen(bool isWindowOpen) { m_windowOpen = isWindowOpen; };

            std::vector<ImGuiWindowFlags_> getWindowFlags() const { return m_flags; };

            void addWindowFlag(ImGuiWindowFlags_ flag) { m_flags.emplace_back(flag); };

            void removeWindowFlag(ImGuiWindowFlags_ flag)
            {
                m_flags.erase(std::remove(m_flags.begin(), m_flags.end(), flag), m_flags.end());
            }

            std::string getWindowTitle() const { return m_windowTitle; };

            void setWindowTitle(std::string newTitle) { m_windowTitle = newTitle; };

        private:
            std::vector<std::shared_ptr<UiElement>> m_content;
            bool m_windowOpen;
            std::vector<ImGuiWindowFlags_> m_flags;
            std::string m_windowTitle = "Hello, World!";
    };
} // namespace Engine
