#version 330 core

out vec4 fragColor;
in vec4 vColor;
in vec2 texCoord;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;
uniform float uAlpha;

void main() {
	fragColor = vColor;
	// * mix(texture(uTexture, texCoord), texture(uTexture2, texCoord), uAlpha);
}