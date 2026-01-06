#include <stdio.h>
#include <cmath>
#define PPI 60
#define FPS 60

typedef struct { float x, y; } vec2;
typedef struct { float x, y, z; } vec3;
struct vec4 {
    float x, y, z, w;
    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(float v) : x(v), y(v), z(v), w(v) {}
    vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
};

vec2 operator*(const vec2 &a, float s) { return {a.x * s, a.y * s}; }
vec2 operator/(const vec2 &a, float s) { return {a.x / s, a.y / s}; }
vec2 operator+(const vec2 &a, float s) { return {a.x + s, a.y + s}; }
vec2 operator-(const vec2 &a, float s) { return {a.x - s, a.y - s}; }
vec3 operator*(const vec3 &a, float s) { return {a.x * s, a.y * s, a.z * s}; }
vec3 operator/(const vec3 &a, float s) { return {a.x / s, a.y / s, a.z / s}; }
vec3 operator+(const vec3 &a, float s) { return {a.x + s, a.y + s, a.z + s}; }
vec3 operator+(const vec3 &a, const vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
vec3 operator-(const vec3 &a, float s) { return {a.x - s, a.y - s, a.z - s}; }
vec3 operator-(const vec3 &a, const vec3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
vec3 sin(const vec3 &v) { return { sin(v.x), sin(v.y), sin(v.z) }; }
vec4 operator/(const vec4 &a, float s) { return {a.x / s, a.y / s, a.z / s, a.w / s}; }
vec4 operator+(const vec4 &a, float s) { return {a.x + s, a.y + s, a.z + s, a.w + s}; }
vec4 operator+(const vec4 &a, const vec4 b) { return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}; }
vec4 tanh(const vec4 &v) { return { tanh(v.x), tanh(v.y), tanh(v.z), tanh(v.w) }; }

vec3 normalize (const vec3 &v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return {v.x / len, v.y / len, v.z / len};
}
vec3 mix (const vec3 &a, const vec3 &b, float t) {
    return a * (1 - t) + b * t;
}
float length (const vec2 &v) {
    return sqrt(v.x * v.x + v.y * v.y);
}
float length (const vec3 &v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
float dot (const vec3 &a, const vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
vec3 cross (const vec3 &a, const vec3 &b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

void shader (int x, int y, vec2 r, FILE *f, float t) {
    vec4 o = vec4(0);
    float z, d, s;
    vec3 FC = {x, y, 0};
    for (float i = 0; i < 70; i ++) {
        o = o + vec4(z, 2, s, 1) / (s * d);
        vec3 p = normalize(vec3{FC.x, FC.y, FC.z} * 2 - vec3{r.x, r.y, r.y}) * z;
        vec3 a;
        p.z += 9;
        a = mix(a * dot(a = a - (float)0.57, p), p, cos(s = s - t)) - cross(a, p) * sin(s);
        s = sqrt(length(vec2{a.x, a.z} - a.y));
        
        for (d = 1; d < 9; d ++) {
            vec3 S = sin(a * d - t);
            a = a + vec3{S.y, S.z, S.x} / d;
            z += d = length(sin(a) + (a.x + a.y + a.z) * 0.2) * s / 20;
        }
    }
    o = tanh(o / 2000.0);
    fputc(o.x * 255, f);
    fputc(o.y * 255, f);
    fputc(o.z * 255, f);
}

int main () {
    char buf[256];
    for (int frame = 0; frame < 60; frame ++) {
        snprintf(buf, sizeof(buf), "output-%02d.ppm", frame);
        const char *output_path = buf;
        FILE *f = fopen(output_path, "wb");
        int w = 16*PPI;
        int h = 9*PPI;
        fprintf(f, "P6\n");
        fprintf(f, "%d %d\n", w, h);
        fprintf(f, "255\n");
        for (int y = 0; y < h; y ++) {
            for (int x = 0; x < w; x ++) {
                shader(x - w / 2, y - h / 2, vec2{(float)w / h, 1} * 1.2, f, frame / FPS);
            }
        }
        printf("Generated %s\n", output_path);
        fclose(f);
    }
    return 0;
}