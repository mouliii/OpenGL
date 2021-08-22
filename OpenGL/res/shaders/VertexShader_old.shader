#version 330 core

layout(location = 0) in vec4 aPosAndTexCoord;
layout(location = 1) in vec4 aColor;
layout(location = 2) in float aTexIndex;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;

uniform mat4 uViewProj;

void main() {
	gl_Position = uViewProj * vec4(aPosAndTexCoord.xy, 0.0f, 1.0f);
	vColor = aColor;
	vTexCoord = aPosAndTexCoord.zw;
	vTexIndex = aTexIndex;
}