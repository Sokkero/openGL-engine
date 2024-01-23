#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    /**
     * @brief Represents a text element in the user interface.
     */
    class UiElementSeparator : public UiElement
    {
        public:
            explicit UiElementSeparator(std::string text) : m_text(std::move(text)) {};
            ~UiElementSeparator() = default;

            void drawUi() override
            {
                ImGui::Spacing();
                if(!m_text.empty())
                {
                    ImGui::SeparatorText(m_text.c_str());
                }
                else
                {
                    ImGui::Separator();
                }
                ImGui::Spacing();
            }

            /**
             * @brief Gets the current text of the UiElementSeparator object.
             *
             * @return The current text.
             */
            std::string getText() const { return m_text; };

            /**
             * @brief Sets the text of the UiElementSeparator object.
             *
             * @param text The new text to be set.
             */
            void setText(std::string text) { m_text = std::move(text); };

        private:
            std::string m_text;
    };
} // namespace Engine::Ui