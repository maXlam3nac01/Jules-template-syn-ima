#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

// layout(location = 0) out vec3 vFragPosition;

out vec3 vFragColor;
out vec2 vFragPosition;

// out vec3 vFragPosition;

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
  vFragPosition = aVertexPosition;
  // gl_Position = vec4(aVertexPosition, 0., 1.);

  // rotate
  // gl_Position = vec4((vec3(aVertexPosition, 1) * rotate(45)).xy, 0., 1.);

  // translate & rotate & scale
  vec2 a_New1_VertexPosition = (vec3(aVertexPosition, 1) * translate(10, 0.) * rotate(90) * scale(0.5f, 0.5f)).xy;
  gl_Position = vec4(a_New1_VertexPosition, 0., 1.);
};