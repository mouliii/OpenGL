#version 330 core

out vec4 fragColor;
in vec4 vColor;

void main()
{
	fragColor = vColor;
}