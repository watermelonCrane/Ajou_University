#version 410 core

out vec4 out_Color;

uniform vec3 cameraPos = vec3(0, 0, 5);
uniform vec3 lightPos = vec3(10, 20, 10);
uniform vec3 lightColor = vec3(1, 1, 1);
uniform vec3 diffColor = vec3(1, 0.5, 0);
uniform float shininess = 50;

in vec3 normal;
in vec3 worldPos;

void main(void)
{
	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPos - worldPos);
	vec3 L = normalize(lightPos - worldPos);
	vec3 R = reflect(-L, N);
	vec3 color = diffColor * lightColor * max(0, dot(N, L));
	color += lightColor*pow(max(0, dot(R,V)), shininess);
	out_Color = vec4(pow(color, vec3(1/2.2)), 1.0);
}
