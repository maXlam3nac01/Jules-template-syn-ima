#version 330 core

uniform mat3 uModelMatrix;

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec2 aVertexText;

out vec2 vText;
out vec2 vFragPosition;

mat3 rotate(float angle_degree){
  float angle_radian = radians(angle_degree);
  return mat3(
    vec3(cos(angle_radian), sin(angle_radian), 0),
    vec3(-sin(angle_radian), cos(angle_radian), 0),
    vec3(0, 0, 1)   
  );
}


void main()
{
    vText      = aVertexText;
    vFragPosition = (uModelMatrix * vec3(aVertexPosition, 1)).xy;
    gl_Position = vec4(vFragPosition, 0., 1.);
}