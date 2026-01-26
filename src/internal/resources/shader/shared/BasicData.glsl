#pragma once

// Input vertex data, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in mat4 modelMatrixInst;

uniform bool isInstanced;
uniform mat4 modelMatrixUni;

layout(std140) uniform ViewProjectionBlock
{
    mat4 viewMat;
    mat4 projMat;
};

vec2 getUV()
{
    return vertexUV;
}

vec3 getNormal()
{
    return vertexNormal;
}

vec3 getModelspace()
{
    return vertexPosition_modelspace;
}

mat4 getModelMatrix()
{
    return isInstanced ? modelMatrixInst : modelMatrixUni;
}

mat4 getMvp()
{
    return projMat * viewMat * getModelMatrix();
}
