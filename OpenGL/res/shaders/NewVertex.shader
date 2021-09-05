#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

out vec4 vColor;
out vec2 vUV;

uniform mat4 uViewProj;
uniform vec4 uColor;
uniform mat4 uTransform;

void main() {
	gl_Position = uViewProj * uTransform * vec4(aPos, 1.0f);
	vColor = aColor * uColor;
	vUV = aUV;
}