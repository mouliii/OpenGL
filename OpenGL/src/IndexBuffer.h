#pragma once
#include "glad.h"
#include <vector>


class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const std::vector<uint32_t>& indices);
	IndexBuffer(const IndexBuffer& vb) = delete;
	IndexBuffer& operator=(const IndexBuffer& vb) = delete;
	~IndexBuffer();
	void SetData(const std::vector<uint32_t>& indices);
	void Bind()const;
	void Unbind()const;
public:
	unsigned int id;
};