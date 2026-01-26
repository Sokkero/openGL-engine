#version 410

#include <"resources/shader/shared/BasicData.glsl">
#include <"resources/shader/shared/AdditionalData.glsl">("vec4")

out vec4 fragmentColor;
out vec3 normal;

void main()
{
    gl_Position = getMvp() * vec4(getModelspace(), 1);

    normal = getNormal();
    fragmentColor = getAdditionalData();
}
