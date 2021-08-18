#pragma once
#include "glad.h"
#include "VertexBuffer.h"
#include "VertexData.h"


class VertexArray
{
public:
	VertexArray();
	VertexArray(const VertexArray& vb) = delete;
	VertexArray& operator=(const VertexArray& vb) = delete;
	~VertexArray();
	void LinkAttribute(const VertexBuffer& vbo, unsigned int location, unsigned int numComponents, GLenum type, unsigned int stride, GLvoid* offset);
	void Bind()const;
	void Unbind()const;
public:
	unsigned int id;
};