#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    /**
     * @brief Represents a radio button UI element.
     */
    class UiElementRadio : public UiElement
    {
        public:
            /**
             * @brief Callback function type for handling radio button value changes.
             * @param value The new value of the radio button.
             */
            using Callback = std::function<void(bool)>;

            /**
             * @brief Constructs a UiElementRadio object.
             * @param startValue The initial value of the radio button.
             * @param text The label text for the radio button.
             * @param callbackFunc The callback function to be called when the value changes.
             */
            UiElementRadio(bool startValue, std::string text, Callback callbackFunc)
                : m_value(startValue)
                , m_previousValue(startValue)
                , m_text(std::move(text))
                , m_onChangeCallback(callbackFunc) {};
            ~UiElementRadio() = default;

            /**
             * @brief Draws the radio button UI element.
             * @details Calls the callback function if the value has changed, then renders the radio button
             * using ImGui.
             */
            void drawUi() override
            {
                if(m_previousValue != m_value)
                {
                    m_onChangeCallback(m_value);
                    m_previousValue = m_value;
                }

                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }
                ImGui::Checkbox(m_text.c_str(), &m_value);
            }

            /**
             * @brief Sets the callback function for value changes.
             * @param func The callback function to be set.
             */
            void setOnChangeCallback(Callback func) { m_onChangeCallback = std::move(func); }

            /**
             * @brief Returns the label text of the radio button.
             * @return The label text.
             */
            std::string getText() const { return m_text; };

            /**
             * @brief Sets the label text of the radio button.
             * @param text The new label text to be set.
             */
            void setText(std::string text) { m_text = std::move(text); };

            bool getValue() const { return m_value; };

            void setValue(bool newValue) { m_value = newValue; };

        private:
            Callback m_onChangeCallback;
            bool m_value;
            bool m_previousValue;
            std::string m_text;
    };
} // namespace Engine::Ui