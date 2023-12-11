#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    /**
     * @brief A UI element for editing color values.
     */
    class UiElementColorEdit : public UiElement
    {
        public:
            /**
             * @brief Callback function type for handling color value changes.
             * @param color The updated color value.
             */
            using Callback = std::function<void(float[4])>;

            /**
             * @brief Constructs a UiElementColorEdit object.
             * @param startValue The initial color value.
             * @param text The text label for the color edit element.
             * @param callbackFunc The callback function to be called when the color value changes.
             * @param flags The ImGui color edit flags.
             */
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

            /**
             * @brief Draws the color edit UI element.
             *        Calls the callback function if the color value has changed.
             */
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

            /**
             * @brief Sets the callback function for handling color value changes.
             * @param func The callback function.
             */
            void setOnChangeCallback(Callback func) { m_onChangeCallback = std::move(func); }

            /**
             * @brief Gets the text label of the color edit element.
             * @return The text label.
             */
            std::string getText() const { return m_text; };

            /**
             * @brief Sets the text label of the color edit element.
             * @param text The text label.
             */
            void setText(std::string text) { m_text = std::move(text); };

            /**
             * @brief Gets the ImGui color edit flags.
             * @return The ImGui color edit flags.
             */
            ImGuiColorEditFlags getFlags() const { return m_flags; };

            /**
             * @brief Sets the ImGui color edit flags.
             * @param flags The ImGui color edit flags.
             */
            void setFlags(ImGuiColorEditFlags flags) { m_flags = flags; };

        private:
            ImGuiColorEditFlags m_flags;
            float m_value[4];
            float m_previousValue[4];
            Callback m_onChangeCallback;
            std::string m_text;

            /**
             * @brief Checks if the color values have changed.
             * @return True if the color values have changed, false otherwise.
             */
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