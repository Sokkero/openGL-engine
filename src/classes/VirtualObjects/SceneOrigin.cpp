
#include "SceneOrigin.h"

#include "../engine/EngineManager.h"
#include "../engine/LightingHelper.h"
#include "../engine/UserEventManager.h"
#include "../engine/WindowManager.h"
#include "CameraActor.h"
#include "TestObject.h"

void SceneOrigin::start()
{
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
    treeObj->setShader(ShaderType::solidTexture, renderManager);
    treeObj->setPosition(glm::vec3(0.f, 0.f, 0));
    treeObj->setTextureBuffer(renderManager->registerTexture("resources/textures/treeTexture.bmp"));
    treeObj->setName("tree");
    // node1->setTint(glm::vec4(1.f, 0.f, 0.f, 1.f));
    addChild(treeObj);
    //treeObj->addChild(camera);

    std::shared_ptr<TestObject> suzanne = std::make_shared<TestObject>();
    suzanne->setObjectData(renderManager->registerObject("resources/objects/suzanne.obj"));
    suzanne->setShader(ShaderType::solidColor, renderManager);
    suzanne->setPosition(glm::vec3(5.f, 0.f, 0));
    suzanne->setScale(glm::vec3(1.f));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < suzanne->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }

    suzanne->setTextureBuffer(renderManager->createVBO(g_color_buffer_data));
    suzanne->setName("suzanne");
    // node1->setTint(glm::vec4(1.f, 0.f, 0.f, 1.f));
    addChild(suzanne);
    //suzanne->addChild(treeObj);
}

void SceneOrigin::update()
{
    if(getUserEventManager()->getUserEvent(GLFW_KEY_V) == GLFW_PRESS)
    {
        getWindowManager()->setVsync(!getWindowManager()->getVsync());
    }
}