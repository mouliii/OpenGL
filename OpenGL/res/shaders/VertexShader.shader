#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 texCoord;

uniform mat4 uMVP;

void main() {
	gl_Position = uMVP * vec4(aPos, 1.0f);
	vColor = aColor;
	texCoord = aTexCoord;
}