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

void VertexArray::LinkAttribute(const VertexBuffer& vbo, unsigned int location, unsigned int numComponents, GLenum type, unsigned int stride, GLvoid* offset)
{
	vbo.Bind();
	glVertexAttribPointer(location, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(location);
	vbo.Unbind();
}
