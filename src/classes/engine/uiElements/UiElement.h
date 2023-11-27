#pragma once

namespace Engine
{
    class UiElement
    {
        public:
            UiElement() = default;
            ~UiElement() = default;

            virtual void drawUi() = 0;

        private:
    };
} // namespace Engine