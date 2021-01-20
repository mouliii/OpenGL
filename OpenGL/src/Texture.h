#pragma once

#include <string>
#include <assert.h>
#include "GLErrorCheck.h"
#include "Renderer.h"

class Texture
{
public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
private:
	unsigned int rendererID;
	std::string filepath;
	unsigned char* localBuffer;
	int width, height, bpp;
};