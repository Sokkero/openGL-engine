#version 410

// Input vertex data, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;

layout(std140) uniform ViewProjectionBlock
{
    mat4 viewMat;
    mat4 projMat;
};

// Values that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform vec4 tintColor;

// Output data ; will be interpolated for each fragment.
out vec4 fragmentColor;
out vec3 normal;

void main()
{
    mat4 mvp = projMat * viewMat * modelMatrix;

    gl_Position = mvp * vec4(vertexPosition_modelspace, 1);

    normal = vertexNormal;
    fragmentColor = vertexColor * tintColor;
}
