#pragma once

#include "classes/SingletonManager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <string>
#include <utility>

namespace Engine
{
    class EngineManager;
    class UserEventManager;

    class WindowManager : public SingletonBase
    {
        public:
            WindowManager();
            ~WindowManager() = default;

            bool startWindow();

            GLFWwindow* getWindow() { return m_gameWindow; };

            glm::vec2 getWindowDimensions() { return m_windowDimensions; };

            void setWindowDimensions(int width, int height);

            int getTextureSamples() const { return m_textureSamples; };

            void setTextureSamples(int samples) { m_textureSamples = samples; }

            std::string getWindowTitle() const { return m_windowTitle; };

            void setWindowTitle(std::string name) { m_windowTitle = std::move(name); };

            bool getVsync() const { return m_vsync; };

            void setVsync(bool);

            void setWindowInputMode(int mode, int value);

        private:
            GLFWwindow* m_gameWindow;
            glm::vec2 m_windowDimensions;
            bool m_vsync;
            int m_textureSamples;
            std::string m_windowTitle;
    };
} // namespace Engine
