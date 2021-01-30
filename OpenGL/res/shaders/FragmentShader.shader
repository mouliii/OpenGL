#version 330 core

out vec4 fragColor;
in vec4 vColor;
in vec2 vTexCoord;
flat in int vTexIndex;

uniform sampler2D uTextures[16];

void main() {
	fragColor = texture(uTextures[vTexIndex], vTexCoord) * vColor;
	fragColor = vColor;
	// * mix(texture(uTexture, texCoord), texture(uTexture2, texCoord), uAlpha);
}