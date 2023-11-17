
#include "MandelbrotScene.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/helper/LightingHelper.h"

#include "../../classes/nodeComponents/GeometryComponent.h"
#include "../engineTestScene/CameraActor.h"

void MandelbrotScene::start()
{
    const auto& renderManager = getEngineManager()->getRenderManager();

    std::shared_ptr<CameraActor> camera = std::make_shared<CameraActor>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 0.f, 1.f));
    camera->setName("camera");
    addChild(camera);
    getEngineManager()->setCamera(camera);

    std::shared_ptr<Engine::GeometryComponent> planeObj = std::make_shared<Engine::GeometryComponent>();
    planeObj->setObjectData(renderManager->registerObject("resources/objects/plane.obj"));
    planeObj->setShader("solidColor");
    planeObj->setPosition(glm::vec3(0.f, 0.f, 0.f));
    planeObj->setName("tree");

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < planeObj->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }
    planeObj->setTextureBuffer(renderManager->createVBO(g_color_buffer_data));

    addChild(planeObj);
}

void MandelbrotScene::update() {}