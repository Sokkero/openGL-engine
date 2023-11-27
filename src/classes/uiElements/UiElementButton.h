#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine
{
    enum ButtonType
    {
        BTN_TYPE_NORMAL = 0,
        BTN_TYPE_SMALL = 1,
        BTN_TYPE_ARROW = 2
    };

    class UiElementButton : protected UiElement
    {
        public:
            using Callback = std::function<void()>;

            UiElementButton(ButtonType type, std::string text, Callback callbackFunc, ImGuiDir dir = ImGuiDir_None)
                : m_text(std::move(text))
                , m_onClickCallback(std::move(callbackFunc))
                , m_buttonType(type)
                , m_arrowDir(dir) {};
            ~UiElementButton() = default;

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

            void setOnClickCallback(Callback func) { m_onClickCallback = std::move(func); }

            ButtonType getButtonType() const { return m_buttonType; };

            void setButtonType(ButtonType type) { m_buttonType = type; };

            ImGuiDir getArrowDir() const { return m_arrowDir; };

            void setArrowDir(ImGuiDir dir) { m_arrowDir = dir; };

            std::string getText() const { return m_text; };

            void setText(std::string text) { m_text = text; };

        private:
            ButtonType m_buttonType;
            ImGuiDir m_arrowDir;
            Callback m_onClickCallback;
            std::string m_text;
    };
} // namespace Engine