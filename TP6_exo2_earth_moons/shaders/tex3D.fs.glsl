#version 330 core

uniform sampler2D uEarthTexture;
uniform sampler2D uMoonTexture;

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;

void main()
{
    vec4 fTextEarth = texture(uEarthTexture, vec2(vTexCoords.x, 1.0 - vTexCoords.y));
    // vec3 normal = normalize(vNormal_vs);

    // Assignation de la couleur du fragment
    // fFragColor = vec4(normal, 1.f);
    fFragColor = fTextEarth;
}