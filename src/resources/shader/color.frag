#version 410

// Input Data
in vec4 fragmentColor;
in vec3 normal;
// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh
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
    vec3 ambientColor = fragmentColor.xyz * vec3(ambientLightColorAndIntensity.xyz * ambientLightColorAndIntensity.w);

    float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
    vec3 diffuseColor = fragmentColor.xyz * vec3(diffuseLightColorAndIntensity.xyz * diffuse * diffuseLightColorAndIntensity.w);

    color = vec4(ambientColor + diffuseColor, fragmentColor.w);
}
