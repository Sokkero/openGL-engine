
#include "DebugLineShader.h"

using namespace Engine;

DebugLineShader::DebugLineShader()
{
    std::shared_ptr<RenderManager> renderManager = SingletonManager::get<RenderManager>();

    registerShader(renderManager, "resources/shader/debugLineShader/debugLine", "debugLine", false, false);
    bindUbo(renderManager->getVpUbo());
}

void DebugLineShader::renderLines(GLuint linesBuffer, int vertexCount)
{
    swapToProgramm();
    loadVertexBuffer(linesBuffer);
    loadModelMatrix(glm::mat4(1.0f));
    drawArray(nullptr, vertexCount, GL_LINES);
}
