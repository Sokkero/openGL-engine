#pragma once

#include <string>
#include <utility>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    class EngineManager;
    class UserEventManager;

    class WindowManager
    {
        public:
            WindowManager();
            ~WindowManager() = default;

            bool startWindow();
            GLFWwindow* getWindow() { return m_gameWindow; };

            std::pair<int, int> getWindowDimensions() { return m_windowDimensions; };
            void setWindowDimensions(int width, int height);

            int getTextureSamples() const { return m_textureSamples; };
            void setTextureSamples(int samples) { m_textureSamples = samples;}

            std::string getWindowTitle() const { return m_windowTitle; };
            void setWindowTitle( std::string name) { m_windowTitle = std::move(name); };

        private:
            GLFWwindow* m_gameWindow;
            std::pair<int, int> m_windowDimensions;
            int m_textureSamples;
            std::string m_windowTitle;
    };
} // namespace Engine
