#version 330 core

out vec4 fragColor;
in vec4 vColor;
in vec2 vTexCoord;

void main()
{
	fragColor = vColor;
}