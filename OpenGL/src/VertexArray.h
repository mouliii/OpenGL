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
	void LinkAttribute(const VertexBuffer& vbo, uint32_t location, uint32_t numComponents, GLenum type, uint32_t stride, GLvoid* offset);
	void Bind()const;
	void Unbind()const;
public:
	uint32_t id;
};