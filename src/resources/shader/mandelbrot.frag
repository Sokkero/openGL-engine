#version 330 core

// Ouput data
out vec4 color;

layout(std140) uniform MandelbrotBlock
{
    int itr;
    float zoom;
    vec2 screenSize;
    vec2 offset;
};

float n = 0.0;
float threshold = 100.0;

float mandelbrot(vec2 c) {
    vec2 z = vec2(0.0,0.0);
    for(int i = 0; i < itr; i++){
        vec2 znew;
        znew.x = (z.x * z.x) - (z.y * z.y) + c.x;
        znew.y = (2.0 * z.x * z.y) +c.y;
        z = znew;
        if((z.x * z.x) + (z.y * z.y) > threshold)break;
        n++;
    }
    return n / float(itr);
}

vec4 map_to_color(float t) {
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1.0);
}

void main() {
    vec2 coord = vec2(gl_FragCoord.xy);
    float t = mandelbrot(((coord - screenSize/2)/zoom) - offset);
    if(gl_FragCoord.x < 40){
        color = vec4(1.0);
    }
    color = map_to_color(float(t));
}