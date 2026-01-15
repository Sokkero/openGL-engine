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
    if(err != GL_NO_ERROR)
    {
        fprintf(stderr, "openGL error occured, code: %d\n", err);
        assert(false);
    }
#endif
}

bool RenderUtils::nodeDistanceSortingAlgorithm(
        const std::shared_ptr<GeometryComponent>& a,
        const std::shared_ptr<GeometryComponent>& b,
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