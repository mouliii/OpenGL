#version 330 core

out vec4 fragColor;
in vec4 vColor;
in vec2 vUV;

uniform sampler2D texture0;

void main()
{
	fragColor = texture(texture0, vUV) * vColor;
}