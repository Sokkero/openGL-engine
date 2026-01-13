#include "MandelbrotDebugWindow.h"

#include "classes/engine/EngineManager.h"
#include "classes/engine/WindowManager.h"
#include "classes/engine/rendering/RenderManager.h"
#include "classes/uiElements/UiElementSlider.h"
#include "classes/uiElements/UiElementText.h"
#include "exampleScenes/mandelbrotScene/MandelbrotUbo.h"

using namespace Engine::Ui;

MandelbrotDebugWindow::MandelbrotDebugWindow(const std::shared_ptr<MandelbrotUbo>& ubo) : m_mandelbrotUbo(ubo)
{
    addWindowFlag(ImGuiWindowFlags_AlwaysAutoResize);

    setWindowTitle("Mandelbrot data");

    addContent(std::make_shared<UiElementText>("Iterations: "));

    int iterations = m_mandelbrotUbo->getIterations();
    const auto& iterationsCallback = ([this](int value) { m_mandelbrotUbo->setIterations(value); });
    std::shared_ptr<UiElementSlider<int>> iterationsEdit =
            std::make_shared<UiElementSlider<int>>(iterations, 0, 500, "x", iterationsCallback);
    iterationsEdit->setSameLine(true);
    addContent(iterationsEdit);
}
