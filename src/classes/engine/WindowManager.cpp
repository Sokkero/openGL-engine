#include "WindowManager.h"

#include "NodeComponents/BasicNode.h"
#include "NodeComponents/CameraComponent.h"
#include "UserEventManager.h"
#include "EngineManager.h"

namespace Engine
{
    WindowManager::WindowManager(std::string windowTitle, int windowWidth, int windowHeight, int textureSamples)
            : m_gameWindow(nullptr)
            , m_windowDimensions(std::pair<int, int>(windowWidth, windowHeight))
            , m_textureSamples(textureSamples)
            , m_windowTitle(std::move(windowTitle))
            , m_engineManager(nullptr)
            , m_userEventManager(nullptr)
    {
    }

    void WindowManager::setWindowDimensions(int width, int height)
    {
        m_windowDimensions = std::pair<int, int>(width, height);
        glfwSetWindowSize(m_gameWindow, width, height);
    }

    int WindowManager::startWindow()
    {
        // Initialise GLFW
        glewExperimental = true; //Needed for core profile
        if (!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW!\n");
            return -1;
        }

        glfwWindowHint(GLFW_SAMPLES, m_textureSamples); // Anti-Aliasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Use version 3.3
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
        if (m_gameWindow == nullptr)
        {
            fprintf(stderr, "Failed to open GLFW window...\n");
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(m_gameWindow); // Initiate GLEW
        glewExperimental = true; // Needed in the core profile
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr, "Failed to initialize GLEW...\n");
            return -1;
        }

        glfwSetInputMode(m_gameWindow, GLFW_STICKY_KEYS, GL_TRUE);

        BasicNode::setWindowManager(this);

        m_userEventManager =  UserEventManager::getUserEventManager();
        m_engineManager = std::make_shared<EngineManager>(EngineManager());

        m_engineManager->getScene()->start();

        do
        {
            m_userEventManager->updateEvents(m_gameWindow);
            m_engineManager->engineUpdate();
            m_engineManager->engineDraw();

            glfwSwapBuffers(m_gameWindow);
            glfwPollEvents();

            m_engineManager->setDeltaTime();
        }
        while (m_userEventManager->getUserEvent(GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_gameWindow) == 0);

        return 0;
    }
}