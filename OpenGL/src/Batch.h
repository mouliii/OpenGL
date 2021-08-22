#pragma once

#include <glad.h>
#include <vector>
#include "VertexData.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OrthoCamera.h"
#include "Primitives.h"

#include "Vec2.h"
#include "Vec3.h"
#include "glm/glm.hpp"

class Batch
{
public:
	Batch(GLenum drawMode, std::string batchName, Shader shader, Primitive primitive, uint32_t maxBatchCount = 10000);
	void Draw(Shader* shader, const OrthoCamera& cam);
	void Add(uint32_t count);
	void Remove();
	void Update(const Primitive& primitive);
	void SetSubData();
	void SetSubData(uint32_t offsetCount, uint32_t count, const void* data);
private:
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	Primitive primitive;
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	//Shader shader; todo

public:
	uint32_t maxBatchCount;
	uint32_t maxNumVertices;
	uint32_t maxNumIndices;
	uint32_t curVertex = 0;
	GLenum drawMode;

	uint32_t numOfDrawCalls = 0;
};