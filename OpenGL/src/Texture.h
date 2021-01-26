#pragma once

#include "glad.h"
#include <memory>
#include <string>
#include <iostream>
#include "stb/stb_image.h"

// TODO mipmaps
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
private:
	int width;
	int height;
	int nChannels;
	unsigned int id = 0;
	GLenum type = GL_DEBUG_TYPE_ERROR;
};