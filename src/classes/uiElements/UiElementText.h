#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    /**
     * @brief Represents a text element in the user interface.
     */
    class UiElementText : public UiElement
    {
        public:
            /**
             * @brief Constructs a UiElementText object with the specified text.
             *
             * @param text The text to be displayed.
             */
            explicit UiElementText(std::string text) : m_text(std::move(text)) {};
            ~UiElementText() = default;

            /**
             * @brief Draws the text element in the user interface.
             */
            void drawUi() override
            {
                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }

                ImGui::Text("%s", m_text.c_str());
            }

            /**
             * @brief Gets the current text of the UiElementText object.
             *
             * @return The current text.
             */
            std::string getText() const { return m_text; };

            /**
             * @brief Sets the text of the UiElementText object.
             *
             * @param text The new text to be set.
             */
            void setText(std::string text) { m_text = std::move(text); };

        private:
            std::string m_text;
    };
} // namespace Engine::Ui