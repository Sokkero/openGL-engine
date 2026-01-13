#include "TestSceneOrigin.h"

#include "classes/engine/EngineManager.h"
#include "classes/engine/WindowManager.h"
#include "classes/primitives/actors/CameraActor.h"
#include "classes/primitives/ui/DebugManagerWindow.h"
#include "exampleScenes/testScene/TestObject.h"
#include "resources/shader/colorShader/ColorShader.h"
#include "resources/shader/textureShader/TextureShader.h"

void TestSceneOrigin::start()
{
    m_engineManager = SingletonManager::get<EngineManager>();

    std::shared_ptr<BasicNode> debugWindow = std::make_shared<Engine::Ui::DebugManagerWindow>();
    debugWindow->setName("debugWindow");
    addChild(debugWindow);

    std::shared_ptr<BasicNode> cameraHolder = std::make_shared<BasicNode>();
    cameraHolder->setName("cameraHolder");
    cameraHolder->setPosition(glm::vec3(0.f, 0.f, 0.f));
    addChild(cameraHolder);

    std::shared_ptr<CameraActor> camera = std::make_shared<CameraActor>();
    camera->setZFar(1000.f);
    camera->setPosition(glm::vec3(0.f, 5.f, 15.f));
    camera->setName("camera");
    cameraHolder->addChild(camera);
    m_engineManager->setCamera(camera);

    const auto& renderManager = m_engineManager->getRenderManager();

    // Tree models normals are broken, causing the model to look bad with translucency
    m_tree = std::make_shared<GeometryComponent>();
    m_tree->setObjectData(renderManager->registerObject("resources/objects/tree.obj"));
    m_tree->setShader(std::make_shared<TextureShader>(renderManager));
    m_tree->setPosition(glm::vec3(0.f, 3.f, 0.f));
    // m_tree->setRotation(glm::vec3(70.f, 0.f, 0.f));
    m_tree->setTextureBuffer(renderManager->registerTexture("resources/textures/treeTexture.bmp"));
    m_tree->setName("tree");
    m_tree->setTint(glm::vec4(1.f, 1.f, 1.f, 1.f));
    addChild(m_tree);

    m_ape = std::make_shared<TestObject>();
    m_ape->setObjectData(renderManager->registerObject("resources/objects/suzanne.obj"));
    m_ape->setShader(std::make_shared<ColorShader>(renderManager));
    m_ape->getShader()->bindUbo(renderManager->getAmbientLightUbo());
    m_ape->setPosition(glm::vec3(3.f, 3.f, 0.f));
    m_ape->setScale(glm::vec3(1.f));
    m_ape->setTint(glm::vec4(1.f, 1.f, 1.f, 1.f));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < m_ape->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(1.f, 1.f, 1.f, 1.f);
    }

    m_ape->setTextureBuffer(RenderUtils::createVertexBufferObject(g_color_buffer_data));
    m_ape->setName("suzanne");
    addChild(m_ape);
}

void TestSceneOrigin::update()
{
    m_tree->rotateObj(glm::vec3(0.f, 1.f, 0.f), m_engineManager->getDeltaTime() * 50.f);
    // m_tree->rotateObj(glm::vec3(1.f, 0.f, 0.f), getEngineManager()->getDeltaTime() * 50.f);
    // m_ape->setPosition(m_tree->getGlobalPosition() + (m_tree->getForward() * 3.f));
}