#pragma once
#include "glad.h"
#include <vector>
#include <iostream>
#include <string>

enum class DataType { NONE = -1, FLOAT, UNSIGNED_INT, INT};

static GLenum GetOpenGLDataType(DataType type)
{
	switch (type)
	{
	case DataType::NONE:
	{
		std::cout << "Invalid DataType at buffer.h" << std::endl;
		return -1;
	}
	case DataType::FLOAT:			return	GL_FLOAT;
	case DataType::UNSIGNED_INT:	return	GL_UNSIGNED_INT;
	case DataType::INT:				return	GL_INT;
	default:
		break;
	}
	return -1;
}

static unsigned int GetDataTypeSize(DataType type)
{
	switch (type)
	{
	case DataType::NONE:
	{
		std::cout << "Invalid DataType at buffer.h" << std::endl;
		return -1;
	}
	case DataType::FLOAT:			return	sizeof(float);
	case DataType::UNSIGNED_INT:	return	sizeof(unsigned int);
	case DataType::INT:				return	sizeof(int);
	default:
		break;
	}
	return -1;
}

struct Attribute
{
	std::string name;
	unsigned int count;
	DataType type;
	bool normalized;
	unsigned int offset = 0;
	Attribute(DataType type, unsigned int count, std::string name, bool normalized = false)
		:
		type(type), count(count), normalized(normalized), name(name)
	{}
};

class BufferLayout
{
public:
	BufferLayout();
	BufferLayout(std::initializer_list<Attribute> attributeList);
	void AddAttribute(Attribute attribute);
	void CalculateStrideAndOffset();
	const unsigned int GetStride() const;
	const int GetAttributeCount() const;
	const std::vector<Attribute> GetAttributes() const;
private:
	std::vector<Attribute> elements;
	unsigned int stride = 0;
	unsigned int offset = 0;
};

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	const void GenerateBuffer();
	void SetData(const void* data, unsigned int size);
	void SetSubData(const void* data, uint32_t size);
	void SetLayout(const BufferLayout& layout);
	const BufferLayout GetLayout()const;
	void Bind()const;
	void Unbind()const;
private:
	unsigned int id;
	BufferLayout layout;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	// bindaa vao ja vbo. sitten looppaa vbo layoutin läpi ja enabloi ne.
	void AddVertexBuffer(const VertexBuffer& vb); 
	void Bind()const;
	void Unbind()const;
private:
	unsigned int id;
};

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();
	void SetData(const void* data, unsigned int count);
	const unsigned int GetCount() const;
	void Bind()const;
	void Unbind()const;
private:
	unsigned int id;
	unsigned int count;
};

