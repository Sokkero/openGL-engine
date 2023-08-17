#version 330 core

// Input Data
in vec2 UV;
in vec3 normal;
// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh
uniform sampler2D textureSampler;
uniform vec4 tintColor = vec4(1.0, 1.0, 1.0, 1.0);
layout(std140) uniform AmbientLightBlock
{
    bool useAmbient;
    float ambientIntensity;
    vec3 ambientLightColor;
};
layout(std140) uniform DiffuseLightBlock
{
    bool useDiffuse;
    float diffuseIntensity;
    vec3 diffuseLightDir;
    vec3 diffuseLightColor;
};

void main()
{
    vec4 textureColor = vec4(texture(textureSampler, UV).rgb, 1) * tintColor;

    vec4 ambientColor;
    if(useAmbient)
    {
        ambientColor = textureColor * vec4(ambientLightColor, 1.0) * ambientIntensity;
    }
    else
    {
        ambientColor = vec4(0.0, 0.0, 0.0, 0.0);
    }

    vec4 diffuseColor;
    if(useDiffuse)
    {
        float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
        diffuseColor = textureColor * vec4(diffuseLightColor, 1.0) * diffuse * diffuseIntensity;
    }
    else
    {
        diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    }

    color = ambientColor + diffuseColor;
}