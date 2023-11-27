#pragma once

#include "imgui.h"

namespace Engine
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

        private:
            bool m_sameLine = false;
            float m_xOffset = 0.f;
            float m_spacing = -1.f;
    };
} // namespace Engine