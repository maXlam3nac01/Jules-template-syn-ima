#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

mat3 translate(float tx, float ty){
  return mat3(
    vec3(1, 0, 0),
    vec3(0, 1, 0),
    vec3(tx, ty, 1)
  );
}

mat3 scale(float sx, float sy){
  return mat3(
    vec3(sx, 0, 0),
    vec3(0, sy, 0),
    vec3(0, 0, 1)
  );
}

mat3 rotate(float angle_degree){
  float angle_radian = radians(angle_degree);
  return mat3(
    vec3(cos(angle_radian), -sin(angle_radian), 0),
    vec3(sin(angle_radian), cos(angle_radian), 0),
    vec3(0, 0, 1)   
  );
}
// déformation car ?

void main() {
  vFragColor = aVertexColor;
  gl_Position = vec4(aVertexPosition, 0., 1.);
};