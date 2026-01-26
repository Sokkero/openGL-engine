#version 410

#include <"resources/shader/shared/Lighting.glsl">

in vec2 UV;
in vec3 normal;
out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    vec4 textureColor = vec4(texture(textureSampler, UV).rgb, 1);

    vec3 ambientColor = textureColor.xyz * vec3(ambientLight.xyz * ambientLight.w);

    float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
    vec3 diffuseColor = textureColor.xyz * vec3(diffuseLight.xyz * diffuse * diffuseLight.w);

    color = vec4(ambientColor + diffuseColor, textureColor.w);
}