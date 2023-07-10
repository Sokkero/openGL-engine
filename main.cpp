
#include <iostream>
#include "src/classes/engine/WindowManager.h"

int main()
{
    Engine::WindowManager WindowManager = Engine::WindowManager();

    int exitCode = WindowManager.startWindow();

    std::cout << "Exited with code " << exitCode << std::endl;
}
