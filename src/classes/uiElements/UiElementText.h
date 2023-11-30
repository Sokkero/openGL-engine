#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    class UiElementText : public UiElement
    {
        public:
            explicit UiElementText(std::string text) : m_text(std::move(text)) {};
            ~UiElementText() = default;

            void drawUi() override
            {
                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }

                ImGui::Text("%s", m_text.c_str());
            }

            std::string getText() const { return m_text; };

            void setText(std::string text) { m_text = std::move(text); };

        private:
            std::string m_text;
    };
} // namespace Engine::Ui