#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 normal;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;
uniform vec4 tintColor = vec4(1.0, 1.0, 1.0, 1.0);

layout(std140) uniform AmbientLightBlock
{
    vec3 ambientLightColor;
    float ambientIntensity;
    int ambientUboIndex;
};

layout(std140) uniform DiffuseLightBlock
{
    bool useDiffuse;
    vec3 diffuseLightDir;
    vec3 diffuseLightColor;
    float diffuseIntensity;
    int diffuseUboIndex;
};

// Ouput data
out vec4 color;

void main()
{
    // Output color = color of the texture at the specified UV
    vec4 ambientColor = vec4(texture(textureSampler, UV).rgb, 1) * vec4(ambientLightColor, 1.0) * ambientIntensity;
    vec4 diffuseColor;

    if(useDiffuse)
    {
        diffuseColor = vec4(diffuseIntensity, diffuseIntensity, diffuseIntensity, 1.0);
        color = vec4(diffuseLightDir, 1.0);
        return;
        //float diffuse = max(dot(normalize(normal), normalize(diffuseLightDir)), 0.0);
        //diffuseColor = vec4(texture(textureSampler, UV).rgb, 1) * vec4(diffuseLightColor, 1.0) * diffuse * diffuseIntensity;
    }
    else
    {
        diffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
        color = diffuseColor;
        return;
    }

    color = (ambientColor + diffuseColor) * tintColor;
}