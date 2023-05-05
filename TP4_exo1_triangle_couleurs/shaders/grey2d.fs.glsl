#version 330 core

in vec3 vFragPosition;
in vec3 vFragColor;

out vec4 fFragColor;

mat3 scale(float sx, float sy){
  return mat3(
    vec3(sx, 0, 0),
    vec3(0, sy, 0),
    vec3(0, 0, 1)
  );
}

float alpha = 0.333;
float beta = 0.333;
float distance = length(vFragPosition - vFragPosition * scale(0.01f, 0.01f));
float attenuate = alpha * exp(-beta * distance * distance);
// float M = (vFragColor.r +vFragColor.g + vFragColor.b) / 3;

void main() {
  fFragColor = vec4(vFragColor * attenuate, 1.);
};