uniform float g_Time;
uniform vec3 g_Screen;
uniform vec4 g_PointerPosition;
uniform vec4 g_PointerPositionLast;
uniform vec4 g_TexelSize;
const vec2 resolution = vec2(1 / g_TexelSize.x, 1 / g_TexelSize.y);

vec2 complexPow (vec2 z, float power) {
    float a = z.x;
    float b = z.y;

    float r = length(z);
    float theta = atan(b / a);

    float r_pow = pow(r, power);

    float theta_pow = theta * power;

    float real = r_pow * cos(theta_pow);
    float imag = r_pow * sin(theta_pow);

    return vec2(real, imag);
}

vec2 transform (vec2 z, vec2 coord, vec2 m) {
    return complexPow(z, 2.0) + m;
}

void main () {
    vec2 uv = gl_Position.xy / resolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;

    vec2 m = g_PointerPosition.xy * 2.0 - 1.0;
    m.y = -m.y;

    vec2 z = uv;
    const int maxIterations = 300;
    int j;
    
    for (int i = 0; i < maxIterations; i ++) {
        if (length(z) > 4.0) {
            j = i;
            break;
        }
        z = transform(z, uv, m);
    }

    float color = float(j) / float(maxIterations);
    vec3 col = vec3(0.2 + 0.2 * cos(3.0 + (color + g_Time / 50.) * 6.28318),
                    0.2 + 0.2 * cos(2.0 + (color + g_Time / 50.) * 6.28318),
                    0.1 + 0.1 * cos(1.0 + (color + g_Time / 50.) * 6.28318));

    gl_FragColor = vec4(col, 1.0);
}