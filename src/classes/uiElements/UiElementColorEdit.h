#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    class UiElementColorEdit : public UiElement
    {
        public:
            using Callback = std::function<void(float[4])>;

            UiElementColorEdit(
                    float startValue[4],
                    std::string text,
                    Callback callbackFunc,
                    ImGuiColorEditFlags flags = ImGuiColorEditFlags_None
            )
                : m_value { startValue[0], startValue[1], startValue[2], startValue[3] }
                , m_previousValue { startValue[0], startValue[1], startValue[2], startValue[3] }
                , m_text(std::move(text))
                , m_onChangeCallback(std::move(callbackFunc))
                , m_flags(flags) {};

            ~UiElementColorEdit() = default;

            void drawUi() override
            {
                if(haveValuesChanged())
                {
                    m_onChangeCallback(m_value);
                    std::copy(std::begin(m_value), std::end(m_value), std::begin(m_previousValue));
                }

                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }

                ImGui::ColorEdit4(m_text.c_str(), m_value, m_flags);
            }

            void setOnChangeCallback(Callback func) { m_onChangeCallback = std::move(func); }

            std::string getText() const { return m_text; };

            void setText(std::string text) { m_text = text; };

            ImGuiColorEditFlags getFlags() const { return m_flags; };

            void setFlags(ImGuiColorEditFlags flags) { m_flags = flags; };

        private:
            ImGuiColorEditFlags m_flags;
            float m_value[4];
            float m_previousValue[4];
            Callback m_onChangeCallback;
            std::string m_text;

            bool haveValuesChanged()
            {
                int i = 0;
                for(const auto& value : m_value)
                {
                    if(value != m_previousValue[i])
                    {
                        return true;
                    }
                    i++;
                }
                return false;
            }
    };
} // namespace Engine::Ui