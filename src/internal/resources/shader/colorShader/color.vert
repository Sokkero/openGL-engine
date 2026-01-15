#version 410

// Input vertex data, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexNormal;

layout(std140) uniform ViewProjectionBlock
{
    mat4 viewMat;
    mat4 projMat;
};

// Values that stay constant for the whole mesh.
layout(location = 3) in mat4 modelMatrix;
layout(location = 7) in vec4 colorData;

// Output data ; will be interpolated for each fragment.
out vec4 fragmentColor;
out vec3 normal;

void main()
{
    mat4 mvp = projMat * viewMat * modelMatrix;

    gl_Position = mvp * vec4(vertexPosition_modelspace, 1);

    normal = vertexNormal;
    fragmentColor = colorData;
}
