#version 410 core

layout(location=0) in vec3 in_Position;

uniform mat4 transform = mat4(1);

void main(void)
{
	vec4 p = transform*vec4(in_Position.xyz, 1.0);
	gl_Position= vec4(p.xyz,1);
}

