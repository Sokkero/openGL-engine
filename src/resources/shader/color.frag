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
    vec3 ambientColor;
    if(useAmbient)
    {
        ambientColor = fragmentColor.xyz * vec3(ambientLightColor * ambientIntensity);
    }
    else
    {
        ambientColor = vec3(0.0, 0.0, 0.0);
    }

    vec3 diffuseColor;
    if(useDiffuse)
    {
        float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
        diffuseColor = fragmentColor.xyz * vec3(diffuseLightColor * diffuse * diffuseIntensity);
    }
    else
    {
        diffuseColor = vec3(0.0, 0.0, 0.0);
    }

    color = vec4(ambientColor + diffuseColor, fragmentColor.w);
}
