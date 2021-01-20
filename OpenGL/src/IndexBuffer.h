#pragma once

#include "GLErrorCheck.h"
#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();
	unsigned int GetCount() const { return count; }
	void Bind()const;
	void Unbind()const;

private:
	unsigned int rendererID;
	unsigned int count;
};