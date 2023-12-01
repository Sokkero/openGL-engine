
#include "MandelbrotSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/nodeComponents/CameraComponent.h"
#include "../../classes/objects/SceneDebugWindow.h"
#include "../../resources/shader/MandelbrotShader.h"
#include "../../resources/shader/SolidColorShader.h"
#include "MandelbrotDebugWindow.h"
#include "MandelbrotUbo.h"

void MandelbrotSceneOrigin::start()
{
    std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::SceneDebugWindow>();
    debugWindow->setName("debugWindow");
    addChild(debugWindow);

    const auto& mandelbrotUbo = std::make_shared<MandelbrotUbo>();
    std::shared_ptr<BasicNode> mandelbrotDebugWindow = std::make_shared<MandelbrotDebugWindow>(mandelbrotUbo);
    mandelbrotDebugWindow->setName("mandelbrotDebugWindow");
    addChild(mandelbrotDebugWindow);

    std::shared_ptr<Engine::CameraComponent> camera = std::make_shared<Engine::CameraComponent>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 0.f, 2.f));
    camera->setName("camera");
    addChild(camera);
    getEngineManager()->setCamera(camera);

    const auto& renderManager = getEngineManager()->getRenderManager();

    std::shared_ptr<Engine::GeometryComponent> mandelbrotPlane = std::make_shared<Engine::GeometryComponent>();
    mandelbrotPlane->setObjectData(renderManager->registerObject("resources/objects/plane.obj"));
    mandelbrotPlane->setShader(std::make_shared<MandelbrotShader>(renderManager, mandelbrotUbo));
    mandelbrotPlane->setPosition(glm::vec3(0.f, 0.f, 0.f));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < mandelbrotPlane->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }

    mandelbrotPlane->setTextureBuffer(renderManager->createVBO(g_color_buffer_data));
    mandelbrotPlane->setName("plane");
    addChild(mandelbrotPlane);

    mandelbrotPlane->getShader()->bindUbo(mandelbrotUbo);
}

void MandelbrotSceneOrigin::update() {}