#version 410

// Input Data
in vec2 UV;
in vec3 normal;
// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh
uniform sampler2D textureSampler;

layout(std140) uniform AmbientLightBlock
{
    vec4 ambientLightColorAndIntensity;
};
layout(std140) uniform DiffuseLightBlock
{
    vec4 diffuseLightColorAndIntensity;
    vec3 diffuseLightDir;
};

void main()
{
    vec4 textureColor = vec4(texture(textureSampler, UV).rgb, 1);

    vec3 ambientColor = textureColor.xyz * vec3(ambientLightColorAndIntensity.xyz * ambientLightColorAndIntensity.w);

    float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
    vec3 diffuseColor = textureColor.xyz * vec3(diffuseLightColorAndIntensity.xyz * diffuse * diffuseLightColorAndIntensity.w);

    color = vec4(ambientColor + diffuseColor, textureColor.w);
}