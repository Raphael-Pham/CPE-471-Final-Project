#version 330 core 

out vec4 color;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float MatShine;

in vec3 fragNor;
in vec3 lightDir1;
in vec3 lightDir2;

//position of the vertex in camera space
in vec3 EPos;

void main()
{
    vec3 normal = normalize(fragNor);
    vec3 light1 = normalize(lightDir1);
    vec3 light2 = normalize(lightDir2);
    vec3 lightCol2 = vec3(0.862, 0.345, 0.145);

    vec3 H1 = normalize(light1 + normalize(EPos));
    vec3 H2 = normalize(light2 + normalize(EPos));

    float dC1 = max(0.0, dot(normal, light1));
    float sC1 = pow(dot(H1, normal), MatShine);
    float dC2 = max(0.0, dot(normal, light2));
    float sC2 = pow(dot(H2, normal), MatShine);

    float dist1 = length(lightDir1);
    float dist2 = length(lightDir2);

    float ambStrength = 1;

    float difStrength1 = 0.3;
    float specStrength1 = 0.3;

    float difStrength2 = 1;
    float specStrength2 = 0.5;

    float c1 = 0.5;
    float c2 = 0.04;

    color = vec4(ambStrength*MatAmb + 
                ((1 / (c1*dist1))*(difStrength1*dC1*MatDif + specStrength1*sC1*MatSpec)) +
                ((1 / (c2*dist2))*(difStrength2*dC2*MatDif + specStrength2*sC2*MatSpec)), 0);
}