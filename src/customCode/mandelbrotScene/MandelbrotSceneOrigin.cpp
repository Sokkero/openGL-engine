
#include "MandelbrotSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/nodeComponents/CameraComponent.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/MandelbrotShader.h"
#include "MandelbrotDebugWindow.h"
#include "MandelbrotUbo.h"

MandelbrotSceneOrigin::MandelbrotSceneOrigin() : m_mandelbrotUbo(nullptr) {}

void MandelbrotSceneOrigin::start()
{
    m_engineManager = SingletonManager::get<Engine::EngineManager>();
    m_userEventManager = SingletonManager::get<Engine::UserEventManager>();

    std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    debugWindow->setName("debugWindow");
    addChild(debugWindow);

    m_mandelbrotUbo = std::make_shared<MandelbrotUbo>();
    m_mandelbrotUbo->setScreenSize(SingletonManager::get<Engine::WindowManager>()->getWindowDimensions());
    std::shared_ptr<BasicNode> mandelbrotDebugWindow = std::make_shared<MandelbrotDebugWindow>(m_mandelbrotUbo);
    mandelbrotDebugWindow->setName("mandelbrotDebugWindow");
    addChild(mandelbrotDebugWindow);

    std::shared_ptr<Engine::CameraComponent> camera = std::make_shared<Engine::CameraComponent>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 0.f, 1.f));
    camera->setName("camera");
    addChild(camera);
    m_engineManager->setCamera(camera);

    const auto& renderManager = m_engineManager->getRenderManager();

    std::shared_ptr<Engine::GeometryComponent> mandelbrotPlane = std::make_shared<Engine::GeometryComponent>();
    mandelbrotPlane->setObjectData(renderManager->registerObject("resources/objects/plane.obj"));
    mandelbrotPlane->setShader(std::make_shared<MandelbrotShader>(renderManager, m_mandelbrotUbo));
    mandelbrotPlane->setPosition(glm::vec3(0.f, 0.f, 0.f));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < mandelbrotPlane->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }

    mandelbrotPlane->setTextureBuffer(RenderUtils::createVertexBufferObject(g_color_buffer_data));
    mandelbrotPlane->setName("plane");
    addChild(mandelbrotPlane);

    mandelbrotPlane->getShader()->bindUbo(m_mandelbrotUbo);
}

void MandelbrotSceneOrigin::increaseZoom()
{
    auto deltaTime = m_engineManager->getDeltaTime();
    auto currZoom = m_mandelbrotUbo->getZoom();
    auto newZoom = currZoom + (currZoom * 2) * deltaTime * 0.25f;

    m_mandelbrotUbo->setZoom(newZoom);
}

void MandelbrotSceneOrigin::decreaseZoom()
{
    auto deltaTime = m_engineManager->getDeltaTime();
    auto currZoom = m_mandelbrotUbo->getZoom();
    auto newZoom = currZoom - (currZoom * 2) * deltaTime * 0.25f;

    m_mandelbrotUbo->setZoom(newZoom);
}

void MandelbrotSceneOrigin::moveCam(glm::vec2 movement)
{
    auto deltaTime = m_engineManager->getDeltaTime();
    auto currZoom = m_mandelbrotUbo->getZoom();
    auto currOffset = m_mandelbrotUbo->getOffset();
    movement *= deltaTime * 400.f;
    auto newOffset = currOffset + movement / currZoom;

    m_mandelbrotUbo->setOffset(newOffset);
}

void MandelbrotSceneOrigin::update()
{
    if(m_userEventManager->getUserEvent(GLFW_KEY_E) == GLFW_PRESS ||
       m_userEventManager->getUserEvent(GLFW_KEY_E) == GLFW_REPEAT)
    {
        increaseZoom();
    }

    if(m_userEventManager->getUserEvent(GLFW_KEY_Q) == GLFW_PRESS ||
       m_userEventManager->getUserEvent(GLFW_KEY_Q) == GLFW_REPEAT)
    {
        decreaseZoom();
    }

    if(m_userEventManager->getUserEvent(GLFW_KEY_R) == GLFW_PRESS ||
       m_userEventManager->getUserEvent(GLFW_KEY_R) == GLFW_REPEAT)
    {
        m_mandelbrotUbo->resetData();
    }

    const glm::vec2 movement = m_userEventManager->getWasdInput();
    if(glm::length(movement) != 0.f)
    {
        moveCam(movement);
    }
}