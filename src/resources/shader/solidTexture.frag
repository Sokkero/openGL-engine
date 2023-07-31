#version 330 core

struct AmbientLight
{
    vec3 lightColor;
    float intensity;
};

// Interpolated values from the vertex shaders
in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;
uniform vec4 tintColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform AmbientLight ambientLight;

// Ouput data
out vec4 color;

void main()
{
    // Output color = color of the texture at the specified UV
    color = vec4(texture(textureSampler, UV).rgb, 1) * tintColor * vec4(ambientLight.lightColor, 1.0)* ambientLight.intensity;
}