#pragma once

#include <glad.h>
#include <vector>
#include "VertexData.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OrthoCamera.h"

#include "Vec2.h"
#include "Vec3.h"
#include "glm/glm.hpp"

class Batch
{
public:
	Batch(GLenum drawMode, std::string batchName, Shader shader, uint32_t maxQuadCount = 1000);
	void BeginFrame();
	void Draw(Shader* shader, const OrthoCamera& cam);
	void Add(uint32_t count);
	void Remove();
	void Update(const std::vector<Vertex>& vertices);
	void SetSubData();
private:
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	Shader shader;

public:
	uint32_t maxQuadCount;
	uint32_t maxNumVertices;
	uint32_t maxNumIndices;
	uint32_t curVertex = 0;
	GLenum drawMode; // glenum ? uint?
};