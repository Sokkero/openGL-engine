#pragma once

#include "../UserEventManager.h"
#include "../WindowManager.h"
#include "../EngineManager.h"

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
            static WindowManager* getWindowManager() { return windowManager; };
        private:
            //TODO: this should hold a vector of shared_ptr's of all its children
            // and also a shared_ptr to its parent
            static WindowManager* windowManager;
    };
    inline WindowManager* BasicActor::windowManager = nullptr;
}