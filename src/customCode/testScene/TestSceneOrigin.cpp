
#include "TestSceneOrigin.h"

#include "../../classes/engine/EngineManager.h"
#include "../../classes/engine/UserEventManager.h"
#include "../../classes/engine/WindowManager.h"
#include "../../classes/primitives/DebugManagerWindow.h"
#include "../../resources/shader/ColorShader.h"
#include "../../resources/shader/TextureShader.h"
#include "CameraActor.h"
#include "TestObject.h"

void TestSceneOrigin::start()
{
    std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    debugWindow->setName("debugWindow");
    addChild(debugWindow);

    std::shared_ptr<BasicNode> cameraHolder = std::make_shared<BasicNode>();
    cameraHolder->setName("cameraHolder");
    cameraHolder->setPosition(glm::vec3(0.f, 0.f, 0.f));
    addChild(cameraHolder);

    std::shared_ptr<CameraActor> camera = std::make_shared<CameraActor>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 5.f, 20.f));
    camera->setName("camera");
    cameraHolder->addChild(camera);
    getEngineManager()->setCamera(camera);

    const auto& renderManager = getEngineManager()->getRenderManager();

    // Tree models normals are broken, causing the model to look bad with translucency
    std::shared_ptr<GeometryComponent> treeObj = std::make_shared<GeometryComponent>();
    treeObj->setObjectData(renderManager->registerObject("resources/objects/tree.obj"));
    treeObj->setShader(std::make_shared<TextureShader>(renderManager));
    treeObj->setPosition(glm::vec3(0.f, -1.5f, 0.f));
    treeObj->setTextureBuffer(renderManager->registerTexture("resources/textures/treeTexture.bmp"));
    treeObj->setName("tree");
    treeObj->setTint(glm::vec4(1.f, 1.f, 1.f, 1.f));
    addChild(treeObj);

    std::shared_ptr<TestObject> suzanneObj = std::make_shared<TestObject>();
    suzanneObj->setObjectData(renderManager->registerObject("resources/objects/suzanne.obj"));
    suzanneObj->setShader(std::make_shared<ColorShader>(renderManager));
    suzanneObj->getShader()->bindUbo(renderManager->getAmbientLightUbo());
    suzanneObj->setPosition(glm::vec3(3.f, -1.5f, 0.f));
    suzanneObj->setScale(glm::vec3(1.f));
    suzanneObj->setTint(glm::vec4(1.f, 1.f, 1.f, 1.f));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < suzanneObj->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }

    suzanneObj->setTextureBuffer(renderManager->createBuffer(g_color_buffer_data));
    suzanneObj->setName("suzanne");
    addChild(suzanneObj);
}

void TestSceneOrigin::update() {}