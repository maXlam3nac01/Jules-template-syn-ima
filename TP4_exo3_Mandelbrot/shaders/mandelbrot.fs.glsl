#version 330

in vec2 vFragPosition;

out vec4 fFragColor;

int N_max = 100;

vec2 complexSqr(vec2 z){
    return vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y);
}

// vec2 Mandelbrot(vec2 vFragPosition, int n){
//     if (n == 0) return vFragPosition;
//     return complexSqr(Mandelbrot(vFragPosition, n-1)) + vFragPosition;
// }

void main()
{
    int n = 0;
    vec2 z = vec2(0.0, 0.0);
    while(n < N_max && length(z) < 2){
        z = complexSqr(z) + vFragPosition;
        n++;
    }

    if (n == N_max){
        fFragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float hue = float(n) / float(N_max);
        vec3 color = vec3(hue, 1, 1);
        fFragColor = vec4(color, 1.0);
    }
}