#pragma once

layout(std140) uniform AmbientLightBlock
{
    vec4 ambientLight;
};

layout(std140) uniform DiffuseLightBlock
{
    vec4 diffuseLight;
    vec3 diffuseLightDir;
};
