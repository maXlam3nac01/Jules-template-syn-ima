#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong()
{
    vec3 color = vec3(0.0);

    float d = distance(vPosition_vs, uLightPos_vs);
    vec3 li = uLightIntensity / (d * d);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 wo = normalize(-vPosition_vs);
    vec3 halfVector = normalize(wi + wo);

    float NdotL = max(dot(vNormal_vs, wi), 0.0);
    float NdotH = max(dot(vNormal_vs, halfVector), 0.0);

    vec3 diffuse = uKd * NdotL;
    vec3 specular = uKs * pow(NdotH, uShininess);

    color = li * (diffuse * specular);

    return color;
}

void main()
{
    fFragColor = vec4(blinnPhong(), 1.0);
}
