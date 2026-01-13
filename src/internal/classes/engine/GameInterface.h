#pragma once

namespace Engine
{
    class GameInterface
    {
        public:
            GameInterface();
            ~GameInterface() = default;

            int startGame();
    };

} // namespace Engine
