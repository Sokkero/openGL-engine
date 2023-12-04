#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    template<typename T>
    class UiElementSlider : public UiElement
    {
        public:
            using Callback = std::function<void(T)>;

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

            float getMinimum() const { return m_minimum; };

            void setMinimum(float min) { m_minimum = min; };

            float getMaximum() const { return m_maximum; };

            void setMaximum(float max) { m_maximum = max; };

            void setOnChangeCallback(Callback func) { m_onChangeCallback = func; }

            std::string getText() const { return m_text; };

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