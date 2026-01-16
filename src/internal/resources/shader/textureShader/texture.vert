#version 410

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

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 normal;

void main()
{
    mat4 modelMatrix = isInstanced ? modelMatrixInst : modelMatrixUni;
    mat4 mvp = projMat * viewMat * modelMatrix;

    // Output position of the vertex, in clip space : MVP * position
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1);

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
    normal = vertexNormal;
}