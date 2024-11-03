#version 410

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
    vec3 ambientColor = mix(vec3(0.0, 0.0, 0.0), fragmentColor.xyz * vec3(ambientLightColor * ambientIntensity), int(useAmbient));

    float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
    vec3 diffuseColor = mix(vec3(0.0, 0.0, 0.0), fragmentColor.xyz * vec3(diffuseLightColor * diffuse * diffuseIntensity), int(useDiffuse));

    color = vec4(ambientColor + diffuseColor, fragmentColor.w);
}
