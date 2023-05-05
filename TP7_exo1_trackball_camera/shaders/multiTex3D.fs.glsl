#version 330 core

uniform sampler2D uTexture; // Texture
uniform sampler2D uTexture2; // Texture

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

void main() {
    fFragColor = vec4(texture(uTexture, vec2(vTexCoords.x, 1.0 - vTexCoords.y)).xyz, 1.);
}
