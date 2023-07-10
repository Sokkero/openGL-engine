#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

namespace Engine {

    class WindowManager {
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

    private:
        GLFWwindow* m_gameWindow;
        std::pair<int, int> m_windowDimensions;
        int m_textureSamples;
        std::string m_windowTitle;
    };
}
