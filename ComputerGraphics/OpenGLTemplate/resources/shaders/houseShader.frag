#version 400 core

in vec2 vTexCoord;
uniform sampler2D sampler0;

in vec3 vColour;
out vec4 vOutputColour;

void main()
{	
	vec4 vTexColour = texture(sampler0, vTexCoord);
	vOutputColour = vTexColour;
}