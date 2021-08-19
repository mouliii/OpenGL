#pragma once
#include "glad.h"
#include <vector>
#include <iostream>
#include <string>
#include "VertexData.h"

class VertexBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(const std::vector<Vertex>& data);
	VertexBuffer(const VertexBuffer& vb) = delete;
	VertexBuffer& operator=(const VertexBuffer& vb) = delete;
	~VertexBuffer();
	void SetData(const std::vector<Vertex>& data);
	void Bind()const;
	void Unbind()const;
public:
	uint32_t id;
};