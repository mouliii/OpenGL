#version 330 core

out vec4 fragColor;
in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D texture0;

void main()
{
	fragColor = texture(texture0, vTexCoord) * vColor;
}