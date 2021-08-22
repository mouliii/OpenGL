#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

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

void VertexBuffer::SetData(uint32_t size, const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::SetSubData(const std::vector<Vertex>& data)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(Vertex), data.data());
}

void VertexBuffer::SetSubData(uint32_t offset, uint32_t count, const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(Vertex), count * sizeof(Vertex), data);
}

void VertexBuffer::SetSubData(uint32_t offset, const std::vector<Vertex>& data)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(Vertex), data.size() * sizeof(Vertex), data.data());
}
