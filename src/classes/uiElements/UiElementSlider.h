#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    /**
     * @brief A template class representing a slider UI element.
     *
     * @tparam T The type of the slider value.
     */
    template<typename T>
    class UiElementSlider : public UiElement
    {
        public:
            /**
             * @brief A callback function type for handling value changes.
             *
             * @param value The new value of the slider.
             */
            using Callback = std::function<void(T)>;

            /**
             * @brief Constructs a new UiElementSlider object.
             *
             * @param startValue The initial value of the slider.
             * @param min The minimum value of the slider.
             * @param max The maximum value of the slider.
             * @param text The text to display next to the slider.
             * @param callbackFunc The callback function to be called when the value changes.
             */
            UiElementSlider(T startValue, T min, T max, std::string text, Callback callbackFunc)
                : m_value(startValue)
                , m_previousValue(startValue)
                , m_text(std::move(text))
                , m_onChangeCallback(std::move(callbackFunc))
                , m_maximum(max)
                , m_minimum(min)
                , m_dataType(ImGuiDataType_COUNT)
            {
                m_dataType = GetDataType(startValue);
            };

            ~UiElementSlider() = default;

            /**
             * @brief Draws the UI element.
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
                ImGui::SliderScalar(m_text.c_str(), m_dataType, &m_value, &m_minimum, &m_maximum);
            }

            /**
             * @brief Gets the minimum value of the slider.
             *
             * @return The minimum value.
             */
            float getMinimum() const { return m_minimum; };

            /**
             * @brief Sets the minimum value of the slider.
             *
             * @param min The new minimum value.
             */
            void setMinimum(float min) { m_minimum = min; };

            /**
             * @brief Gets the maximum value of the slider.
             *
             * @return The maximum value.
             */
            float getMaximum() const { return m_maximum; };

            /**
             * @brief Sets the maximum value of the slider.
             *
             * @param max The new maximum value.
             */
            void setMaximum(float max) { m_maximum = max; };

            /**
             * @brief Sets the callback function to be called when the value changes.
             *
             * @param func The callback function.
             */
            void setOnChangeCallback(Callback func) { m_onChangeCallback = func; }

            /**
             * @brief Gets the text displayed next to the slider.
             *
             * @return The text.
             */
            std::string getText() const { return m_text; };

            /**
             * @brief Sets the text to be displayed next to the slider.
             *
             * @param text The new text.
             */
            void setText(std::string text) { m_text = std::move(text); };

        private:
            ImGuiDataType_ m_dataType;
            Callback m_onChangeCallback;
            T m_value;
            T m_previousValue;
            std::string m_text;
            T m_minimum;
            T m_maximum;
    };
} // namespace Engine::Ui