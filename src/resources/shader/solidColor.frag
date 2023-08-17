#version 330 core

// Input Data
in vec4 fragmentColor;
in vec3 normal;
// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh
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
    vec4 ambientColor;
    if(useAmbient)
    {
        ambientColor = fragmentColor * vec4(ambientLightColor, 1.0) * ambientIntensity;
    }
    else
    {
        ambientColor = vec4(0.0, 0.0, 0.0, 0.0);
    }

    vec4 diffuseColor;
    if(useDiffuse)
    {
        float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
        diffuseColor = fragmentColor * vec4(diffuseLightColor, 1.0) * diffuse * diffuseIntensity;
    }
    else
    {
        diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    }

    color = ambientColor + diffuseColor;
}
