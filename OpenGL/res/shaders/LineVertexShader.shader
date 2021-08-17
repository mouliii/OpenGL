#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;
out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;

uniform mat4 uViewProj;

void main() {
	gl_Position = uViewProj * vec4(aPos.xy, 0.0f, 1.0f);
	vColor = aColor;
}