#pragma once

#include "EngineManager.h"
#include "UserEventManager.h"
#include "WindowManager.h"

#include "string"

namespace Engine
{

    class GameInterface
    {
        public:
            GameInterface();
            ~GameInterface() = default;

            int startGame();

            std::shared_ptr<WindowManager> getWindowManager() const { return m_windowManager; };

            std::shared_ptr<EngineManager> getEngineManager() const { return m_engineManager; };

            std::shared_ptr<UserEventManager> getUserEventManager() const { return m_userEventManager; };

        private:
            std::shared_ptr<WindowManager> m_windowManager;
            std::shared_ptr<EngineManager> m_engineManager;
            std::shared_ptr<UserEventManager> m_userEventManager;
    };

} // namespace Engine
