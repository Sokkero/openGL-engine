#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    class UiElementCollapsableHeader : public UiElement
    {
        public:
            explicit UiElementCollapsableHeader(std::string header) : m_header(std::move(header)) {};
            ~UiElementCollapsableHeader() = default;

            void drawUi() override
            {
                if(ImGui::CollapsingHeader(m_header.c_str()))
                {
                    for(const auto& item : m_sectionContent)
                    {
                        item->drawUi();
                    }
                };
            }

            std::string getHeader() const { return m_header; };

            void setHeader(std::string header) { m_header = std::move(header); };

        private:
            std::string m_header;
            std::vector<std::shared_ptr<UiElement>> m_sectionContent;
    };
} // namespace Engine::Ui