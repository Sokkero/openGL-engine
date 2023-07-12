
#include "src/classes/engine/WindowManager.h"

#include <iostream>

int main()
{
    Engine::WindowManager* WindowManager = new Engine::WindowManager();

    int exitCode = WindowManager->startWindow();

    std::cout << "Exited with code " << exitCode << std::endl;
}
