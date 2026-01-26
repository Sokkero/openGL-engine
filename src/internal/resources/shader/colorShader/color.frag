#version 410

#include <"resources/shader/shared/Lighting.glsl">

in vec4 fragmentColor;
in vec3 normal;

out vec4 color;


void main()
{
    vec3 ambientColor = fragmentColor.xyz * vec3(ambientLight.xyz * ambientLight.w);

    float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
    vec3 diffuseColor = fragmentColor.xyz * vec3(diffuseLight.xyz * diffuse * diffuseLight.w);

    color = vec4(ambientColor + diffuseColor, fragmentColor.w);
}
