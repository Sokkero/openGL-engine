
#include "WafeFunctionCollapseSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/ColorShader.h"

void WafeFunctionCollapseSceneOrigin::start()
{
    getWindowManager()->setWindowDimensions(1024, 768);

    std::shared_ptr<Engine::CameraComponent> camera = std::make_shared<Engine::CameraComponent>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 10.f, 0.f));
    camera->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    camera->setName("camera");
    addChild(camera);
    getEngineManager()->setCamera(camera);

    const auto& renderManager = getEngineManager()->getRenderManager();

    // Tree models normals are broken, causing the model to look bad with translucency
    std::shared_ptr<Engine::GeometryComponent> planeObj = std::make_shared<Engine::GeometryComponent>();
    planeObj->setObjectData(renderManager->registerObject("resources/objects/plane.obj"));
    planeObj->setShader(std::make_shared<ColorShader>(renderManager));
    planeObj->setName("tree");
    planeObj->setRotation(glm::vec3(-90.f, 0.f, 0.f));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < planeObj->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 0.f, 0.f, 1.f);
    }

    planeObj->setTextureBuffer(renderManager->createBuffer(g_color_buffer_data));
    addChild(planeObj);
}

void WafeFunctionCollapseSceneOrigin::update()
{
    // getEngineManager()->getCamera()->rotateObj(getEngineManager()->getCamera()->getRight(), getEngineManager()->getDeltaTime() * 50);
}