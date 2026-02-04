#version 410

#include <"resources/shader/shared/BasicData.glsl">

void main()
{
    gl_Position = getMvp() * vec4(getModelspace(), 1);
}
