// Credits to @MarieEveDube2
// https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

#version 410

layout(std140) uniform ViewProjectionBlock
{
    mat4 viewMat;
    mat4 projMat;
};

out vec3 nearPoint;
out vec3 farPoint;

out mat4 fragView = viewMat;
out mat4 fragProj = projMat;

// Grid position are in xy clipped space
vec3 gridPlane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 proj) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(proj);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = gridPlane[gl_VertexID];
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, viewMat, projMat); // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0, viewMat, projMat); // unprojecting on the far plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}
