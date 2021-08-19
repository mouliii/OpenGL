#pragma once

#include <glad.h>
#include <vector>
#include "VertexData.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// ei vielä käytössä
struct BatchConfig
{
	GLenum mode;
	//uint32_t layer = 0;
	//uint32_t textureId = 0;
	// matrix ? attribute vai uniform
}

// modeen oma struct?enum
// laita index bufferi manuaalisesi batch size

class Batch
{
public:
	Batch(GLenum drawMode, std::string batchName, const std::vector<Vertex>& vertices);
	void Draw();
	void Add(const std::vector<Vertex>& vertices);
	void Update();
private:
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;

public:
	uint32_t maxNumVertices;
	uint32_t usedNumVertices;
	int drawMode; // glenum ? uint?
};