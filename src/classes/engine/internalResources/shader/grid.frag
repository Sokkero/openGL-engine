#version 330 core

in vec3 nearPoint; // nearPoint calculated in vertex shader
in vec3 farPoint;

layout(location = 0) out vec4 color;

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    color = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0)); // opacity = 1 when t > 0, opacity = 0 otherwise
}
