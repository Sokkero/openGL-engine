#pragma once

namespace Engine
{
    class WindowManager;

    class BasicActor
    {
        public:
            BasicActor() = default;
            virtual ~BasicActor() = default;

            virtual void start() {};
            virtual void update() {};

            static void setWindowManager(WindowManager* newWindowManager) { windowManager = newWindowManager; };
        private:
            static WindowManager* windowManager;
    };
    inline WindowManager* BasicActor::windowManager = nullptr;
}