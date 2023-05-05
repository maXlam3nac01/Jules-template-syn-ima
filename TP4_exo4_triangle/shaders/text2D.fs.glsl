#version 330 core

uniform vec3 uColor;
uniform sampler2D uTexture;

in vec2 vText;

out vec3 fFragText;

void main()
{
    vec4 fText = texture(uTexture, vText);
    fFragText = fText.rgb * uColor;
}