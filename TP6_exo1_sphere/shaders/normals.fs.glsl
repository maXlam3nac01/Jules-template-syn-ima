#version 330 core

in vec3 vNormal_vs;

out vec4 fFragColor;

void main()
{
    vec3 normal = normalize(vNormal_vs);

    // Assignation de la couleur du fragment
    fFragColor = vec4(normal, 1.f);

}