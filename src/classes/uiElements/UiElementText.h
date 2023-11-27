#pragma once

#include "UiElement.h"

#include <string>

namespace Engine
{
    class UiElementText : protected UiElement
    {
        public:
            UiElementText(std::string text) : m_text(std::move(text)) {};
            ~UiElementText() = default;

            void drawUi() override
            {
                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }
                ImGui::Text("%s", m_text.c_str());
            }

        private:
            std::string m_text;
    };
} // namespace Engine