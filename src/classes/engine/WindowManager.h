#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    class EngineManager;
    class UserEventManager;

    class WindowManager
    {
        public:
            explicit WindowManager(
                    std::string windowTitle = "My little Engine",
                    int windowWidth = 1024,
                    int windowHeight = 640,
                    int textureSamples = 4
            );
            ~WindowManager() = default;

            int startWindow();
            void setWindowDimensions(int width, int height);
            GLFWwindow* getWindow() { return m_gameWindow; };
            std::pair<int, int> getWindowDimensions() { return m_windowDimensions; };

            std::shared_ptr<EngineManager> getEngine() { return m_engineManager; };
            std::shared_ptr<UserEventManager> getUserEventManager() { return m_userEventManager; };

        private:
            GLFWwindow* m_gameWindow;
            std::shared_ptr<EngineManager> m_engineManager;
            std::shared_ptr<UserEventManager> m_userEventManager;
            std::pair<int, int> m_windowDimensions;
            int m_textureSamples;
            std::string m_windowTitle;
    };
}
