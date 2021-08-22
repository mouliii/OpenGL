#version 330 core

out vec4 fragColor;
in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;

uniform sampler2D uTextures[16];

void main()
{
	fragColor = texture(uTextures[int(vTexIndex)], vTexCoord) * vColor;
	//fragColor.x += 0.5f;
	//fragColor = vec4(1.f, 1.f, 1.f, 1.f);
	
	//fragColor = vColor;
	// * mix(texture(uTexture, texCoord), texture(uTexture2, texCoord), uAlpha);
}