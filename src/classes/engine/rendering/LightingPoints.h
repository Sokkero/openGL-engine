#pragma once

#include "GL/glew.h"
#include "utility"

namespace Engine
{
    inline const std::pair<const char*, GLuint> AMBIENT_LIGHT_POINT =
            std::pair<const char*, GLuint>("AmbientLightBlock", 45);
    inline const std::pair<const char*, GLuint> DIFFUSE_LIGHT_POINT =
            std::pair<const char*, GLuint>("DiffuseLightBlock", 47);
} // namespace Engine