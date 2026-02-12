#version 410 core

layout(location=0) in vec3 in_Position;
layout(location=2) in vec3 in_Normal;	// normalVector Buffer 전달받기

uniform mat4 modelMat = mat4(1);
uniform mat4 viewMat = mat4(1);
uniform mat4 projMat = mat4(1);

out vec3 normal;	// fs로 normalVector 전달
out vec3 worldPos;

void main(void)
{
	normal = normalize(modelMat*vec4(in_Normal, 0)).xyz;	// world coordinate 상의 normalvector 계산, 단위벡터로 normalize
	vec4 p = projMat*viewMat*modelMat*vec4(in_Position.xyz, 1.0);
	worldPos = (modelMat*vec4(in_Position.xyz, 1.0)).xyz;
	gl_Position= p;
}
 
