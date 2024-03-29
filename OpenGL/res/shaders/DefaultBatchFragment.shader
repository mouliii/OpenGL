#version 330 core

out vec4 fragColor;
in vec4 vColor;
in vec2 vUV;
in float vTexIndex;

uniform sampler2D uTextures[16];

void main()
{
	fragColor = texture(uTextures[int(vTexIndex)], vUV) * vColor;
}