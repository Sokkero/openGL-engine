#version 410

#include <"resources/shader/shared/BasicData.glsl">

out vec2 UV;
out vec3 normal;

void main()
{
    gl_Position = getMvp() * vec4(getModelspace(), 1);

    UV = getUV();
    normal = getNormal();
}