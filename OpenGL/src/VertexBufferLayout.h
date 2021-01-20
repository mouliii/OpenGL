#pragma once

#include <GL/glew.h>
#include <vector>


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		default:
			std::cout << "VERTEX BUFFER ELEMENT: UNSPECIFIED TYPE - " << typeid(type).name() << " -" << std::endl;
			break;
		}
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
		:
			stride(0)
		{}

	inline unsigned int GetStride() const { return stride; }
	inline const std::vector<VertexBufferElement>& GetElements() const { return elements; }

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}
	template<>
	void Push<float>(unsigned int count)
	{
		elements.push_back({GL_FLOAT, count, GL_FALSE});
		stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
};