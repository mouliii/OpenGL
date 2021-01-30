#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in int aTexIndex;

out vec4 vColor;
out vec2 vTexCoord;
flat out int vTexIndex;

uniform mat4 uViewProj;

void main() {
	gl_Position = uViewProj * vec4(aPosition, 1.0f);
	vColor = aColor;
	vTexCoord = aTexCoord;
	vTexIndex = aTexIndex;
}