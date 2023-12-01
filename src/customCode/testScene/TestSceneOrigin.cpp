
#include "TestSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/engine/rendering/lighting/LightingPoints.h"
#include "../../classes/objects/SceneDebugWindow.h"
#include "../../resources/shader/SolidColorShader.h"
#include "../../resources/shader/SolidTextureShader.h"
#include "CameraActor.h"
#include "TestObject.h"

#include <imgui.h>

void TestSceneOrigin::start()
{
    std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::SceneDebugWindow>();
    debugWindow->setName("debugWindow");
    addChild(debugWindow);

    std::shared_ptr<BasicNode> cameraHolder = std::make_shared<BasicNode>();
    cameraHolder->setName("cameraHolder");
    cameraHolder->setPosition(glm::vec3(0.f, 0.f, 0.f));
    addChild(cameraHolder);

    std::shared_ptr<CameraActor> camera = std::make_shared<CameraActor>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 0.f, 15.f));
    camera->setName("camera");
    cameraHolder->addChild(camera);
    getEngineManager()->setCamera(camera);

    const auto& renderManager = getEngineManager()->getRenderManager();

    std::shared_ptr<GeometryComponent> treeObj = std::make_shared<GeometryComponent>();
    treeObj->setObjectData(renderManager->registerObject("resources/objects/tree.obj"));
    treeObj->setShader(std::make_shared<SolidTextureShader>(renderManager));
    treeObj->setPosition(glm::vec3(0.f, 0.f, 0.f));
    treeObj->setTextureBuffer(renderManager->registerTexture("resources/textures/treeTexture.bmp"));
    treeObj->setName("tree");
    treeObj->setTint(glm::vec3(1.f, 1.f, 1.f));
    addChild(treeObj);

    std::shared_ptr<TestObject> suzanneObj = std::make_shared<TestObject>();
    suzanneObj->setObjectData(renderManager->registerObject("resources/objects/suzanne.obj"));
    suzanneObj->setShader(std::make_shared<SolidColorShader>(renderManager));
    suzanneObj->getShader()->bindUbo(renderManager->getAmbientLightUbo());
    suzanneObj->setPosition(glm::vec3(3.f, 0.f, 0.f));
    suzanneObj->setScale(glm::vec3(1.f));
    suzanneObj->setTint(glm::vec3(1.f, 0.f, 0.f));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < suzanneObj->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }

    suzanneObj->setTextureBuffer(renderManager->createVBO(g_color_buffer_data));
    suzanneObj->setName("suzanne");
    addChild(suzanneObj);
}

void TestSceneOrigin::update() {}