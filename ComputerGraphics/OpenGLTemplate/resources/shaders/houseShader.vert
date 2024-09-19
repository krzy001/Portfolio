#version 400 core

uniform struct Matrices 
{
	mat4 projMatrix;
	mat4 modelViewMatrix;
	mat3 normalMatrix;
} matrices;

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

uniform LightInfo light1;
uniform MaterialInfo material1;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 vTexCoord;

out vec3 vColour;

void main() {

	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0f);

	vTexCoord = inCoord;
}