#pragma once

#include "../nodeComponents/CameraComponent.h"
#include "EngineManager.h"

#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <utility>

namespace Engine
{
    class WindowEventCallbackHelper
    {
        public:
            static std::shared_ptr<EngineManager> ENIGNE_MANAGER;

            static void executeWindowResizeCallbacks(GLFWwindow* window, int width, int height);
            static void addWindowResizeCallback(const std::string& id, std::function<void(int, int)> callback);
            static void removeWindowResizeCallback(const std::string& id);

        private:
            static std::vector<std::pair<std::string, std::function<void(int, int)>>> WINDOW_RESIZE_CALLBACKS;
    };
} // namespace Engine