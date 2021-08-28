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
#include "Texture.h"
#include "TextureManager.h"

struct RenderStats
{
	uint32_t drawCalls;
	uint32_t triangles;
	// textures
	// jtn
};

class Renderer
{
public:
	Renderer();
	void Draw( Primitive& primitive,const OrthoCamera& cam, const Texture* tex = nullptr, Shader* shader = nullptr);
	void Draw( Primitive& primitive, const OrthoCamera& cam, const std::shared_ptr<uint32_t> tex, Shader* shader = nullptr);
private:
	Primitive primitive;
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	Shader shader;
};