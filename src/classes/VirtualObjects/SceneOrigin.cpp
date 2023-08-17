
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
    cameraHolder->setPosition(glm::vec3(0.f, -0.f, 0.f));
    addAsChild(cameraHolder);

    std::shared_ptr<CameraActor> camera = std::make_shared<CameraActor>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 0.f, -15.f));
    camera->setName("camera");
    cameraHolder->addAsChild(camera);
    getEngineManager()->setCamera(camera);

    const auto& renderManager = getEngineManager()->getRenderManager();

    std::shared_ptr<TestObject> node1 = std::make_shared<TestObject>();
    node1->setObjectData(renderManager->registerObject("resources/objects/tree.obj"));
    node1->setShader(ShaderType::solidTexture, renderManager);
    node1->setPosition(glm::vec3(-3.f, -2.f, 0));
    node1->setTextureBuffer(renderManager->registerTexture("resources/textures/treeTexture.bmp"));
    node1->setName("tree");
    // node1->setTint(glm::vec4(1.f, 0.f, 0.f, 1.f));
    addAsChild(node1);

    std::shared_ptr<TestObject> node2 = std::make_shared<TestObject>();
    node2->setObjectData(renderManager->registerObject("resources/objects/suzanne.obj"));
    node2->setShader(ShaderType::solidColor, renderManager);
    node2->setPosition(glm::vec3(3.f, -1.f, 0));

    std::vector<glm::vec4>  g_color_buffer_data;
    for (int v = 0; v < node2->getObjectData()->getVertexCount() ; v++){
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }

    node2->setTextureBuffer(renderManager->createVBO(g_color_buffer_data));
    node2->setName("cube");
    // node1->setTint(glm::vec4(1.f, 0.f, 0.f, 1.f));
    addAsChild(node2);
}