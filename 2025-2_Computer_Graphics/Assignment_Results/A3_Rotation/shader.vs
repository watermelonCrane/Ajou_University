#version 410 core

layout(location=0) in vec3 in_Position;

uniform mat3 transform = mat3(1);

void main(void)
{
	gl_Position= vec4((transform*vec3(in_Position.xy, 1.0)).xy, 0, 1);
}

