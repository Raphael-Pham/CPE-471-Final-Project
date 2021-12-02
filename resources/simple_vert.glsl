#version 330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec3 lightPos1;
uniform vec3 lightPos2;

//keep these and set them correctly
out vec3 fragNor;
out vec3 lightDir1;
out vec3 lightDir2;
out vec3 EPos;

void main()
{
	gl_Position = P * V * M * vertPos;
	
	fragNor = (V * M * vec4(vertNor, 0.0)).xyz; 
	lightDir1 = lightPos1 - (V * M * vertPos).xyz;
    lightDir2 = lightPos2 - (M * vertPos).xyz;
	EPos = -1 * (V * M * vertPos).xyz;
}