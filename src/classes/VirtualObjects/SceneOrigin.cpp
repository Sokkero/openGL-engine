
#include "SceneOrigin.h"

#include "../engine/NodeComponents/BasicNode.h"
#include "../engine/WindowManager.h"
#include "../engine/UserEventManager.h"
#include "../engine/EngineManager.h"
#include "TestObject.h"
#include "CameraActor.h"

void SceneOrigin::start()
{
    setName("origin");
    m_engineManager = getWindowManager()->getEngine();

    std::shared_ptr<BasicNode> cameraHolder = std::make_shared<BasicNode>();
    cameraHolder->setName("cameraHolder");
    addAsChild(cameraHolder);

    std::shared_ptr<CameraActor> camera = std::make_shared<CameraActor>();
    camera->setPosition(glm::vec3(0.f, 0.f, -15.f));
    camera->setName("camera");
    cameraHolder->addAsChild(camera);
    m_engineManager->setCamera(camera);

    std::vector<glm::vec4>  g_color_buffer_data;
    for (int v = 0; v < 12*3 ; v++){
        g_color_buffer_data.emplace_back(.55f, .2f, .2f, 1.f);
    }

    std::shared_ptr<TestObject> node1 = std::make_shared<TestObject>();
    node1->setObjectData(m_engineManager->getRenderManager()->registerObject("resources/objects/cube.obj"));
    node1->setShader(ShaderType::solidTexture, m_engineManager->getRenderManager());
    node1->setPosition(glm::vec3(0, 0, 0));
    node1->setTextureBuffer(m_engineManager->getRenderManager()->registerTexture("resources/textures/uvtemplate.bmp"));
    node1->setName("obj1");
    addAsChild(node1);

    /*
    std::shared_ptr<TestObject> node2 = std::make_shared<TestObject>();
    node2->setObjectData(m_engineManager->getRenderManager()->registerObject("resources/objects/cube.obj"));
    node2->setShader(ShaderType::solidColor, m_engineManager->getRenderManager());
    node2->setPosition(glm::vec3(-10, 0, 0));
    node2->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    node2->setTextureBuffer(m_engineManager->getRenderManager()->createVBO(g_color_buffer_data));
    node2->setName("obj2");
    node1->addAsChild(node2);
    */
}