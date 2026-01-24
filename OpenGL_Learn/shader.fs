#version 410 core

out vec4 out_Color;

uniform vec3 cameraPos = vec3(0, 0, 5);
uniform vec3 lightPos = vec3(10, 20, 10);
uniform vec3 lightColor = vec3(1, 1, 1);
uniform vec3 diffColor = vec3(1, 0.5, 0);
uniform float shininess = 50;

in vec3 normal;
in vec3 worldPos;


uniform sampler2D diffTex;
uniform sampler2D bumpTex;

in vec2 texCoord;

mat3 getTBN(vec3 N) {
	vec3 Q1 = dFdx(worldPos), Q2 = dFdy(worldPos);
	vec2 st1 = dFdx(texCoord), st2 = dFdy(texCoord);
	float D = st1.s*st2.t-st2.s*st1.t;
	return mat3(normalize((Q1*st2.t - Q2*st1.t)*D), normalize((-Q1*st2.s + Q2*st1.s)*D), N);
}

void main(void)
{

	vec3 N = normalize(normal);

	const float TEX_DELTA = 0.0001;
	mat3 TBN = getTBN(N);
	float Bu = texture(bumpTex, texCoord+vec2(TEX_DELTA, 0)).r - texture(bumpTex, texCoord-vec2(TEX_DELTA, 0)).r;
	float Bv = texture(bumpTex, texCoord+vec2(0, TEX_DELTA)).r - texture(bumpTex, texCoord-vec2(0, TEX_DELTA)).r;
	vec3 bumpVec = vec3(-Bu*15., -Bv*15., 1);
	N = normalize(TBN*bumpVec);

	vec3 V = normalize(cameraPos - worldPos);
	vec3 L = normalize(lightPos - worldPos);
	vec3 R = reflect(-L, N);

	vec3 color = texture(diffTex, texCoord).rgb * lightColor * max(0, dot(N, L));
	color += lightColor*pow(max(0, dot(R,V)), shininess);
	

	out_Color = vec4(pow(color, vec3(1/2.2)), 1.0);
}
