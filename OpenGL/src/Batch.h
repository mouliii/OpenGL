#pragma once

#include <glad.h>
#include <vector>
#include "VertexData.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OrthoCamera.h"
#include "Mesh.h"
#include "TextureManager.h"

#include "Vec2.h"
#include "Vec3.h"
#include "glm/glm.hpp"

class Batch
{
public:
	Batch(std::string batchName, Primitive batchType, uint32_t maxBatchCount = 10000);
	void BeginBatch();
	void EndBatch();
	void Flush();
	void Add(uint32_t count, const Primitive& primitive);
	void Remove();
	void Update(const std::vector<Vertex>& vertices, const std::shared_ptr<Texture>& texture);
	void SetSubData(uint32_t offsetCount, uint32_t count, const void* data);
private:
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> textureSlots;
	Primitive primitive;
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	uint32_t textureSlotIndex = 1;
	Shader shader;

public:
	uint32_t maxBatchCount;
	uint32_t maxNumVertices;
	uint32_t maxNumIndices;
	uint32_t curVertexCount = 0;

	uint32_t numOfDrawCalls = 0;

	//testi
	OrthoCamera* cam = nullptr;
};