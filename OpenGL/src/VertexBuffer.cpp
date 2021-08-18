#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const std::vector<Vertex>& data)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), data.data(), GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}
void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const Vertex vertData[])
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_DYNAMIC_DRAW);
}