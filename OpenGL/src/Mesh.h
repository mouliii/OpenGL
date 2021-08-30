#pragma once
#include <vector>
#include "VertexData.h"
#include "Primitives.h"
#include "Renderer.h"
#include "OrthoCamera.h"

class Mesh
{
public:
	Mesh(Primitive& primitive, Texture& texturePath )
	{
		vertices = primitive.GetVertices();
		indices = primitive.GetIndices();
	}
	void Update()
	{
		// pos ? rot yms
	}
	void Draw(Renderer& renderer, const OrthoCamera& camera)
	{
		//renderer.Draw()
	}
	// set uv
	// set texture
private:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::shared_ptr<uint32_t> textureId;
};