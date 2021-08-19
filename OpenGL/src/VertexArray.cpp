#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}
void VertexArray::Bind() const
{
	glBindVertexArray(id);
}
void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::LinkAttribute(const VertexBuffer& vbo, uint32_t location, uint32_t numComponents, GLenum type, uint32_t stride, GLvoid* offset)
{
	vbo.Bind();
	glVertexAttribPointer(location, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(location);
	vbo.Unbind();
}
