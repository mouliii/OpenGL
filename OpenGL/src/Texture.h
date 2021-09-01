#pragma once

#include "glad.h"
#include <memory>
#include <string>
#include <iostream>
#include "stb/stb_image.h"

// todo
// subregion
// texture manager

class Texture
{
public:
	Texture(const std::string& filepath, GLenum type = GL_TEXTURE_2D);
	~Texture();
	void Bind() const;
	void Unbind() const;
	void Delete() const;
	const uint32_t GetId() const;
	const int GetWidth() const { return width; };
	const int GetHeight() const { return height; }
private:
	int width;
	int height;
	int nChannels;
	uint32_t id = 0;
	GLenum type = GL_TEXTURE_2D;
};