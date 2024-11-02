#version 410

// Usage of doubles not possible on macOS :(

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

float mandelbrot(vec2 complexPoint) {
    vec2 complexValue = vec2(0.0, 0.0);

    for (int i = 0; i < itr; i++) {
        vec2 updatedValue;
        updatedValue.x = (complexValue.x * complexValue.x) - (complexValue.y * complexValue.y) + complexPoint.x;
        updatedValue.y = (2.0 * complexValue.x * complexValue.y) + complexPoint.y;
        complexValue = updatedValue;

        if ((complexValue.x * complexValue.x) + (complexValue.y * complexValue.y) > threshold) {
            break;
        }

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
    float mandelbrotValue = mandelbrot(((coord - screenSize)/zoom) - offset);
    color = map_to_color(float(mandelbrotValue));
}