#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    class UiElementRadio : public UiElement
    {
        public:
            using Callback = std::function<void(bool)>;

            UiElementRadio(bool startValue, std::string text, Callback callbackFunc)
                : m_value(startValue)
                , m_previousValue(startValue)
                , m_text(std::move(text))
                , m_onChangeCallback(std::move(callbackFunc)) {};
            ~UiElementRadio() = default;

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

            void setOnChangeCallback(Callback func) { m_onChangeCallback = std::move(func); }

            std::string getText() const { return m_text; };

            void setText(std::string text) { m_text = text; };

        private:
            Callback m_onChangeCallback;
            bool m_value;
            bool m_previousValue;
            std::string m_text;
    };
} // namespace Engine::Ui