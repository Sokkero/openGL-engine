#include "WindowManager.h"

#include "EngineManager.h"
#include "NodeComponents/BasicNode.h"
#include "NodeComponents/CameraComponent.h"
#include "UserEventManager.h"

#include <iostream>

namespace Engine
{
    WindowManager::WindowManager()
        : m_gameWindow(nullptr)
        , m_windowDimensions(std::pair<int, int>(1024, 640))
        , m_textureSamples(4)
        , m_windowTitle("My little Engine")
    {
    }

    void WindowManager::setWindowDimensions(int width, int height)
    {
        m_windowDimensions = std::pair<int, int>(width, height);
        glfwSetWindowSize(m_gameWindow, width, height);
    }

    void WindowManager::setVsync(bool vsync)
    {
        m_vsync = vsync;
        glfwSwapInterval(m_vsync);
    }

    bool WindowManager::startWindow()
    {
        // Initialise GLFW
        glewExperimental = true; // Needed for core profile
        if(!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW!\n");
            return false;
        }

        glfwWindowHint(GLFW_SAMPLES, m_textureSamples); // Anti-Aliasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Use version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_gameWindow = glfwCreateWindow(
                m_windowDimensions.first,
                m_windowDimensions.second,
                m_windowTitle.c_str(),
                nullptr,
                nullptr
        );
        if(m_gameWindow == nullptr)
        {
            fprintf(stderr, "Failed to open GLFW window...\n");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_gameWindow); // Initiate GLEW
        glewExperimental = true;              // Needed in the core profile
        if(glewInit() != GLEW_OK)
        {
            fprintf(stderr, "Failed to initialize GLEW...\n");
            return false;
        }

        glfwSetInputMode(m_gameWindow, GLFW_STICKY_KEYS, GL_TRUE);

        std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

        return true;
    }
} // namespace Engine