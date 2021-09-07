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
#include "Mesh.h"
#include "Texture.h"
#include "TextureManager.h"
 
struct RenderStats
{
	uint32_t drawCalls;
	uint32_t triangles;
	// textures
	// jtn
	void ResetStats() { drawCalls = 0; triangles = 0; };
};

class Renderer
{
public:
	Renderer(OrthoCamera* camera);
	void BeginFrame();
	void EndFrame();
	void DrawTexture(const Vec2f& pos, const Texture* texture);
	void Draw(Primitive& primitive, const Texture* texture);
	void Draw(Primitive& primitive, const glm::vec4& color);
	void Draw(Primitive& primitive, const Texture* texture, glm::mat4& transform, Shader& shader);
	void Draw(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Texture* texture);
	void Draw(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const glm::vec4& color);
	void Draw(Mesh& mesh);
private:
	void ImmediateDraw(Primitive& primitive);
	void ImmediateDraw(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
private:
	Primitive primitive;
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	Shader shader;
	std::shared_ptr<Texture> defaultTexture;

	OrthoCamera* camera;
};