#include "WindowEventCallbackHelper.h"

using namespace Engine;

std::vector<std::pair<std::string, std::function<void(int, int)>>> WindowEventCallbackHelper::WINDOW_RESIZE_CALLBACKS = std::vector<std::pair<std::string, std::function<void(int, int)>>>();
std::shared_ptr<EngineManager> WindowEventCallbackHelper::ENIGNE_MANAGER = nullptr;

void WindowEventCallbackHelper::executeWindowResizeCallbacks(GLFWwindow* window, int width, int height)
{

    if(!ENIGNE_MANAGER || !ENIGNE_MANAGER->getCamera())
    {
        return;
    }

    ENIGNE_MANAGER->getCamera()->setAspectRatio((float)width / (float)height);

    for(const auto& callback : WINDOW_RESIZE_CALLBACKS)
    {
        callback.second(width, height);
    }
}

void WindowEventCallbackHelper::addWindowResizeCallback(const std::string& id, std::function<void(int, int)> callback)
{
    const auto& it = std::find_if(
            WINDOW_RESIZE_CALLBACKS.begin(),
            WINDOW_RESIZE_CALLBACKS.end(),
            [id](const std::pair<std::string, std::function<void(int, int)>>& data)
            {
                return data.first == id;
            });

    if(it == WINDOW_RESIZE_CALLBACKS.end())
    {
        WINDOW_RESIZE_CALLBACKS.push_back(std::make_pair(id, callback));
    }
}

void WindowEventCallbackHelper::removeWindowResizeCallback(const std::string& id)
{
    WINDOW_RESIZE_CALLBACKS.erase(
            std::remove_if(
                    WINDOW_RESIZE_CALLBACKS.begin(),
                    WINDOW_RESIZE_CALLBACKS.end(),
                    [id](const std::pair<std::string, std::function<void(int, int)>>& data)
                    {
                        return data.first == id;
                    }
            ),
            WINDOW_RESIZE_CALLBACKS.end()
    );
}