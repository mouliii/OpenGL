#include "Buffer.h"


////////////////////////////////
///		VERTEX ARRAY		 ///
////////////////////////////////
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vb)
{
	glBindVertexArray(id);
	vb.Bind();
	const std::vector<Attribute>& attributes = vb.GetLayout().GetAttributes();
	for (size_t i = 0; i < attributes.size(); i++)
	{
		glVertexAttribPointer(
			i, attributes[i].count, GetOpenGLDataType(attributes[i].type), attributes[i].normalized ? GL_TRUE : GL_FALSE,
			vb.GetLayout().GetStride(), (const void*)attributes[i].offset);
		glEnableVertexAttribArray(i);
	}
}

void VertexArray::Bind()const
{
	glBindVertexArray(id);
}

void VertexArray::Unbind()const
{
	glBindVertexArray(0);
}

////////////////////////////////
///		VERTEX BUFFER		 ///
////////////////////////////////

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}

const void VertexBuffer::GenerateBuffer()
{
	glGenBuffers(1, &id);
}

void VertexBuffer::SetData(const void* data, unsigned int size)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::SetSubData(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::SetLayout(const BufferLayout& layout)
{
	this->layout = layout;
}

const BufferLayout VertexBuffer::GetLayout() const
{
	return layout;
}

void VertexBuffer::Bind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

////////////////////////////////
///		INDEX BUFFER		 ///
////////////////////////////////

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	:
	count(count)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &id);
	count = 0;
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &id);
}

void IndexBuffer::SetData(const void* data, unsigned int count)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);
	this->count = count;
}

const unsigned int IndexBuffer::GetCount() const
{
	return count;
}

void IndexBuffer::Bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


////////////////////////////////
///		BUFFER LAYOUT		 ///
////////////////////////////////

BufferLayout::BufferLayout()
{
}

BufferLayout::BufferLayout(std::initializer_list<Attribute> attributeList)
	:
	elements(attributeList)
{
	CalculateStrideAndOffset();
}

void BufferLayout::AddAttribute(Attribute attribute)
{
	elements.push_back(attribute);
	CalculateStrideAndOffset();
}

void BufferLayout::CalculateStrideAndOffset()
{
	offset = 0;
	stride = 0;
	for (auto& element : elements)
	{
		element.offset = offset;
		offset += GetDataTypeSize(element.type) * element.count;
		stride += GetDataTypeSize(element.type) * element.count;
	}
}

const unsigned int BufferLayout::GetStride() const
{
	return stride;
}

const int BufferLayout::GetAttributeCount()const
{
	return elements.size();
}

const std::vector<Attribute> BufferLayout::GetAttributes() const
{
	return elements;
}