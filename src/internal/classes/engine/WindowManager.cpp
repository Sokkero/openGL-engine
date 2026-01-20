#include "WindowManager.h"

#include "EngineManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <iostream>

namespace Engine
{
    std::vector<std::pair<std::string, WindowManager::Callback>> WindowManager::FRAME_BUFFER_RESIZE_CALLBACKS;

    WindowManager::WindowManager()
        : m_gameWindow(nullptr)
        , m_windowDimensions(glm::ivec2(1200, 600))
        , m_textureSamples(4)
        , m_windowTitle("My little Engine")
        , m_vsync(false)
    {
    }

    void WindowManager::setWindowDimensions(int width, int height)
    {
        m_windowDimensions = glm::ivec2(width, height);
        glfwSetWindowSize(m_gameWindow, width, height);
    }

    void WindowManager::setVsync(bool vsync)
    {
        m_vsync = vsync;
        glfwSwapInterval(m_vsync);
        LOG_DEBUG("WindowManager", stringf("Vsync %s", m_vsync ? "on" : "off"));
    }

    bool WindowManager::startWindow()
    {
        // Initialise GLFW
        if(!glfwInit())
        {
            ENGINE_ASSERT(false, "Failed to initialize GLFW!")
            return false;
        }

        glfwWindowHint(GLFW_SAMPLES, m_textureSamples); // Anti-Aliasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Use version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

        m_gameWindow = glfwCreateWindow(
                m_windowDimensions.x,
                m_windowDimensions.y,
                m_windowTitle.c_str(),
                nullptr,
                nullptr
        );
        if(!m_gameWindow)
        {
            ENGINE_ASSERT(false, "Failed to open GLFW window...");
            glfwTerminate();
            return false;
        }

        glfwPollEvents();

        glfwMakeContextCurrent(m_gameWindow); // Initiate GLEW
        glewExperimental = true;              // Needed in the core profile
        if(glewInit() != GLEW_OK)
        {
            ENGINE_ASSERT(false, "Failed to initialize GLEW...");
            glfwTerminate();
            return false;
        }

        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(m_gameWindow, &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

        LOG_DEBUG("WindowManager", stringf("Window size: %ix%i", m_windowDimensions.x, m_windowDimensions.y));
        LOG_DEBUG("WindowManager", stringf("Framebuffer size: %ix%i", framebufferWidth, framebufferHeight));

        glfwSetFramebufferSizeCallback(m_gameWindow, ExecuteFramebufferSizeCallbacks);

        glfwSwapInterval(m_vsync);

        glfwSetInputMode(m_gameWindow, GLFW_STICKY_KEYS, GL_TRUE);
        // glfwSetInputMode(m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        const unsigned char* raw = glGetString(GL_VERSION);
        std::string str = reinterpret_cast<const char*>(raw);
        LOG_DEBUG("WindowManager", stringf("Using OpenGL %s", str));

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
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

    void WindowManager::AddFramebufferResizeCallback(const std::string& callbackId, const Callback& callback)
    {
        for(const auto& func : FRAME_BUFFER_RESIZE_CALLBACKS)
        {
            if(func.first == callbackId)
            {
                LOG_WARN("WindowManager", stringf("Callback with ID %s already added!", callbackId));
                return;
            }
        }
        FRAME_BUFFER_RESIZE_CALLBACKS.push_back({ callbackId, callback });
    }

    void WindowManager::RemoveFramebufferResizeCallback(const std::string& callbackId)
    {
        FRAME_BUFFER_RESIZE_CALLBACKS
                .erase(std::remove_if(
                               FRAME_BUFFER_RESIZE_CALLBACKS.begin(),
                               FRAME_BUFFER_RESIZE_CALLBACKS.end(),
                               [callbackId](const std::pair<std::string, Callback>& data)
                               { return data.first == callbackId; }
                       ),
                       FRAME_BUFFER_RESIZE_CALLBACKS.end());
    }

    void WindowManager::ExecuteFramebufferSizeCallbacks(GLFWwindow* window, int width, int height)
    {
        LOG_DEBUG("WindowManager", stringf("Framebuffer resized to: %sx%s", width, height));
        glViewport(0, 0, width, height);

        for(const auto& callback : FRAME_BUFFER_RESIZE_CALLBACKS)
        {
            callback.second(window, width, height);
        }
    }

    void WindowManager::setWindowInputMode(int mode, int value)
    {
        glfwSetInputMode(m_gameWindow, mode, value);
    }
} // namespace Engine