#version 410

// Input vertex data, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;

layout(std140) uniform ViewProjectionBlock
{
    mat4 viewMat;
    mat4 projMat;
};

uniform mat4 modelMatrix;

void main()
{
    mat4 mvp = projMat * viewMat * modelMatrix;
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1);
}