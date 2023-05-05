#version 330

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec3 aVertexColor;

out vec3 vColor;
out vec2 vFragPosition;

void main()
{
    vColor      = aVertexColor;
    vFragPosition = aVertexPosition; // On assigne la position à la variable de sortie
    gl_Position = vec4(aVertexPosition, 0., 1.);
}