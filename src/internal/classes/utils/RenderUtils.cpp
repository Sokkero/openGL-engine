#include "RenderUtils.h"

#include "classes/engine/rendering/Shader.h"

#include <GL/glew.h>
#include <assert.h>
#include <glm/vec3.hpp>
#include <memory>
#include <stdio.h>

using namespace Engine;

void RenderUtils::checkForGLError()
{
#ifdef DEBUG
    GLenum err = glGetError();
    ENGINE_ASSERT(err == GL_NO_ERROR, stringf("openGL error occured, code: %d", err))
#endif
}

bool RenderUtils::nodeDistanceSortingAlgorithm(
        const std::shared_ptr<RenderComponent>& a,
        const std::shared_ptr<RenderComponent>& b,
        const glm::vec3& cameraPosition
)
{
    const auto& aTranslucency = a->getIsTranslucent();
    if(aTranslucency != b->getIsTranslucent())
    {
        return !aTranslucency;
    }
    else if(!aTranslucency)
    {
        return a->getShader()->getShaderIdentifier().second < b->getShader()->getShaderIdentifier().second;
    }

    const auto& distanceA = glm::distance(a->getGlobalPosition(), cameraPosition);
    const auto& distanceB = glm::distance(b->getGlobalPosition(), cameraPosition);

    return distanceA > distanceB;
}