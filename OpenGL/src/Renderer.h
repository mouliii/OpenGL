#pragma once

#include "glm/glm.hpp"
#include "Buffer.h"
#include "Shader.h"

struct RenderStats
{
	unsigned int drawCalls = 0;
	unsigned int quadCount = 0;
};

struct QuadVertex
{
	glm::vec3 position;
	glm::vec4 color;
	//glm::vec2 texCoord;
};

struct RenderData2D
{
	const uint32_t maxQuadCount = 10000;
	const uint32_t maxVertexCount = maxQuadCount * 4; // quadissa 4 verticeä
	const uint32_t maxIndicesCount = maxQuadCount * 6;  // 3 pistettä per kolmio -> 6 neliö
	const uint32_t maxTextures = 32;					// kato paljon on maximi?

	QuadVertex* VertexBufferBase = nullptr;
	QuadVertex* VertexBufferPtr = nullptr;

	unsigned int quadIndexCount = 0;

};

static RenderData2D data;
static RenderStats stats;

class Renderer
{
public:
	Renderer();
	~Renderer();
	void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
	void DrawQuad(const glm::vec3& pos, const glm::vec2& size, unsigned int textureID);
	const RenderStats GetRenderStats() const;
	void ResetStats();
	void Clear();
	void BeginBatch();
	void Flush();
	void EndBatch();
	// todo - väliaikaisesti public
	Shader shader;
private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	glm::vec4 clearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
};