#pragma once

#include "classes/SingletonManager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <string>
#include <utility>

namespace Engine
{
    class EngineManager;
    class UserEventManager;

    class WindowManager : public SingletonBase
    {
        public:
            using Callback = std::function<void(GLFWwindow*, int, int)>;

            WindowManager();
            ~WindowManager() = default;

            bool startWindow();

            GLFWwindow* getWindow() { return m_gameWindow; };

            glm::ivec2 getWindowDimensions() { return m_windowDimensions; };
            void setWindowDimensions(int width, int height);

            int getTextureSamples() const { return m_textureSamples; };
            void setTextureSamples(int samples) { m_textureSamples = samples; }

            std::string getWindowTitle() const { return m_windowTitle; };
            void setWindowTitle(std::string name) { m_windowTitle = std::move(name); };

            bool getVsync() const { return m_vsync; };
            void setVsync(bool);

            void setWindowInputMode(int mode, int value);

            static void AddFramebufferResizeCallback(const std::string& callbackId, const Engine::WindowManager::Callback& callback);
            static void RemoveFramebufferResizeCallback(const std::string& callbackId);

        private:
            static void ExecuteFramebufferSizeCallbacks(GLFWwindow*, int width, int height);

            static std::vector<std::pair<std::string, Callback>> FRAME_BUFFER_RESIZE_CALLBACKS;

            GLFWwindow* m_gameWindow;
            glm::ivec2 m_windowDimensions;
            bool m_vsync;
            int m_textureSamples;
            std::string m_windowTitle;
    };
} // namespace Engine
