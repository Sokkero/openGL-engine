#pragma once

#include "imgui.h"

#include <utility>

namespace Engine::Ui
{
    /**
     * @brief Base class for UI elements.
     */
    class UiElement
    {
        public:
            UiElement() = default;
            ~UiElement() = default;

            /**
             * @brief Pure virtual function to draw the UI element. Override is necessary.
             */
            virtual void drawUi() = 0;

            /**
             * @brief Check if the UI element should be drawn on the same line as the previous element.
             * @return true if the UI element should be drawn on the same line, false otherwise.
             */
            bool isSameLine() const { return m_sameLine; };

            /**
             * @brief Set whether the UI element should be drawn on the same line as the previous element.
             * @param sameLine true if the UI element should be drawn on the same line, false otherwise.
             */
            void setSameLine(bool sameLine) { m_sameLine = sameLine; };

            /**
             * @brief Get the X offset of the UI element.
             * @return The X offset of the UI element.
             */
            float getXOffset() const { return m_xOffset; };

            /**
             * @brief Set the X offset of the UI element.
             * @param offset The X offset of the UI element.
             */
            void setXOffset(float offset) { m_xOffset = offset; };

            /**
             * @brief Get the spacing between UI elements.
             * @return The spacing between UI elements.
             */
            float getSpacing() const { return m_spacing; };

            /**
             * @brief Set the spacing between UI elements.
             * @param spacing The spacing between UI elements.
             */
            void setSpacing(float spacing) { m_spacing = spacing; };

            /**
             * @brief Get the ImGuiDataType for a given type.
             * @tparam T The type for which to get the ImGuiDataType.
             * @param value An instance of the type T (not used).
             * @return The ImGuiDataType for the given type T.
             */
            template<typename T>
            static ImGuiDataType_ GetDataType(const T)
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