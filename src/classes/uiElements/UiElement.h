#pragma once

#include "imgui.h"

#include <utility>

namespace Engine::Ui
{
    class UiElement
    {
        public:
            UiElement() = default;
            ~UiElement() = default;

            virtual void drawUi() = 0;

            bool isSameLine() const { return m_sameLine; };

            void setSameLine(bool sameLine) { m_sameLine = sameLine; };

            float getXOffset() const { return m_xOffset; };

            void setXOffset(float offset) { m_xOffset = offset; };

            float getSpacing() const { return m_spacing; };

            void setSpacing(float spacing) { m_spacing = spacing; };

            template<typename T>
            static ImGuiDataType_ GetDataType(const T data)
            {
                if constexpr(std::is_same_v<T, char>)
                {
                    return ImGuiDataType_S8;
                }
                else if constexpr(std::is_same_v<T, short>)
                {
                    return ImGuiDataType_S16;
                }
                else if constexpr(std::is_same_v<T, int>)
                {
                    return ImGuiDataType_S32;
                }
                else if constexpr(std::is_same_v<T, long>)
                {
                    return ImGuiDataType_S64;
                }
                else if constexpr(std::is_same_v<T, float>)
                {
                    return ImGuiDataType_Float;
                }
                else if constexpr(std::is_same_v<T, double>)
                {
                    return ImGuiDataType_Double;
                }

                return ImGuiDataType_COUNT;
            }

        private:
            bool m_sameLine = false;
            float m_xOffset = 0.f;
            float m_spacing = -1.f;
    };
} // namespace Engine::Ui