#pragma once

#include "UiElement.h"

#include <string>
#include <utility>
#include <vector>

namespace Engine::Ui
{
    class UiElementCollapsableSection : public UiElement
    {
        public:
            using Callback = std::function<void(bool open)>;

            explicit UiElementCollapsableSection(std::string header)
                : m_header(std::move(header))
                , m_isOpen(false) {};
            ~UiElementCollapsableSection() = default;

            void drawUi() override
            {
                const bool isOpen = ImGui::CollapsingHeader(m_header.c_str());
                if(isOpen)
                {
                    for(const auto& item : m_sectionContent)
                    {
                        item->drawUi();
                    }
                }

                if(isOpen != m_isOpen)
                {
                    swapState();
                }
            }

            std::string getHeader() const { return m_header; };

            std::vector<std::shared_ptr<UiElement>>& getSectionContent() { return m_sectionContent; };

            void addContent(const std::shared_ptr<UiElement>& item) { m_sectionContent.push_back(item); };

            void setCallback(Callback callback) { m_stateChangeCallback = std::move(callback); };

            bool getIsCurrentlyOpen() const { return m_isOpen; }

        private:
            void swapState()
            {
                m_isOpen = !m_isOpen;

                if(m_stateChangeCallback)
                {
                    m_stateChangeCallback(m_isOpen);
                }
            }

            Callback m_stateChangeCallback;
            bool m_isOpen;
            std::string m_header;
            std::vector<std::shared_ptr<UiElement>> m_sectionContent;
    };
} // namespace Engine::Ui