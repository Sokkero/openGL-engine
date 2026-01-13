#pragma once

#include "UiElement.h"

#include <functional>
#include <string>
#include <utility>

namespace Engine::Ui
{
    /**
     * @brief Enum representing the type of button.
     */
    enum ButtonType
    {
        BTN_TYPE_NORMAL = 0,
        BTN_TYPE_SMALL = 1,
        BTN_TYPE_ARROW = 2
    };

    /**
     * @brief A class representing a UI button element.
     */
    class UiElementButton : public UiElement
    {
        public:
            /**
             * @brief Callback function type for button click event.
             */
            using Callback = std::function<void()>;

            /**
             * @brief Constructs a UiElementButton object.
             * @param type The type of button.
             * @param text The text to be displayed on the button.
             * @param callbackFunc The callback function to be called when the button is clicked.
             * @param dir The direction of the arrow button (only applicable for BTN_TYPE_ARROW).
             */
            UiElementButton(ButtonType type, std::string text, Callback callbackFunc, ImGuiDir dir = ImGuiDir_None)
                : m_text(std::move(text))
                , m_onClickCallback(std::move(callbackFunc))
                , m_buttonType(type)
                , m_arrowDir(dir) {};
            ~UiElementButton() = default;

            /**
             * @brief Draws the UI button element.
             */
            void drawUi() override
            {
                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }

                switch(m_buttonType)
                {
                    case BTN_TYPE_NORMAL:
                        if(ImGui::Button(m_text.c_str()))
                        {
                            m_onClickCallback();
                        }
                        break;
                    case BTN_TYPE_SMALL:
                        if(ImGui::SmallButton(m_text.c_str()))
                        {
                            m_onClickCallback();
                        }
                        break;
                    case BTN_TYPE_ARROW:
                        if(ImGui::ArrowButton(m_text.c_str(), m_arrowDir))
                        {
                            m_onClickCallback();
                        }
                        break;
                }
            }

            /**
             * @brief Sets the callback function for button click event.
             * @param func The callback function.
             */
            void setOnClickCallback(Callback func) { m_onClickCallback = std::move(func); }

            /**
             * @brief Gets the type of the button.
             * @return The button type.
             */
            ButtonType getButtonType() const { return m_buttonType; };

            /**
             * @brief Sets the type of the button.
             * @param type The button type.
             */
            void setButtonType(ButtonType type) { m_buttonType = type; };

            /**
             * @brief Gets the direction of the arrow button.
             * @return The arrow direction.
             */
            ImGuiDir getArrowDir() const { return m_arrowDir; };

            /**
             * @brief Sets the direction of the arrow button.
             * @param dir The arrow direction.
             */
            void setArrowDir(ImGuiDir dir) { m_arrowDir = dir; };

            /**
             * @brief Gets the text displayed on the button.
             * @return The button text.
             */
            std::string getText() const { return m_text; };

            /**
             * @brief Sets the text to be displayed on the button.
             * @param text The button text.
             */
            void setText(std::string text) { m_text = std::move(text); };

        private:
            ButtonType m_buttonType;
            ImGuiDir m_arrowDir;
            Callback m_onClickCallback;
            std::string m_text;
    };
} // namespace Engine::Ui