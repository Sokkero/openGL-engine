#version 410

// Input Data
in vec2 UV;
in vec3 normal;
// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh
uniform sampler2D textureSampler;
uniform vec4 tintColor;
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

    vec3 ambientColor = mix(vec3(0.0, 0.0, 0.0), textureColor.xyz * vec3(ambientLightColor * ambientIntensity), int(useAmbient));

    float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
    vec3 diffuseColor = mix(vec3(0.0, 0.0, 0.0), textureColor.xyz * vec3(diffuseLightColor * diffuse * diffuseIntensity), int(useDiffuse));

    color = vec4(ambientColor + diffuseColor, textureColor.w);
}