#include "WindowManager.h"

#include "EngineManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

namespace Engine
{
    WindowManager::WindowManager()
        : m_gameWindow(nullptr)
        , m_windowDimensions(std::pair<int, int>(1024, 640))
        , m_textureSamples(4)
        , m_windowTitle("My little Engine")
        , m_vsync(true)
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
        std::cout << "Vsync " << (m_vsync ? "on" : "off") << std::endl;
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

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_gameWindow, true);
        ImGui_ImplOpenGL3_Init("#version 150");

        return true;
    }
} // namespace Engine