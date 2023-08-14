#version 330 core

// Input vertex data, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec4 tintColor = vec4(1.0, 1.0, 1.0, 1.0);

// Output data ; will be interpolated for each fragment.
out vec4 fragmentColor;
out vec3 normal;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    normal = vertexNormal;
    fragmentColor = vertexColor * tintColor;
}
