#pragma once

#include "../../classes/nodeComponents/UiDebugWindow.h"

class MandelbrotUbo;

class MandelbrotDebugWindow : public Engine::Ui::UiDebugWindow
{
    public:
        MandelbrotDebugWindow(std::shared_ptr<MandelbrotUbo> ubo);
        ~MandelbrotDebugWindow() = default;

    private:
        std::shared_ptr<MandelbrotUbo> m_mandelbrotUbo;
};