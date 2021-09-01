#pragma once
#include <vector>
#include "VertexData.h"
#include "Primitives.h"
#include "TextureManager.h"
#include "Vec2.h"

class Mesh
{
public:
	Mesh(Primitive& primitive )
	{
		vertices = primitive.GetVertices();
		indices = primitive.GetIndices();
	}
	void Update()
	{
		// pos ? rot yms
	}

	void LoadTexture(const std::string& path)
	{
		texture = TextureManager::LoadTexture(path);
	}
	void SetTextureUV(Vec2f botLeft, Vec2f topRight)
	{
		vertices[0].texCoord = { botLeft.x, botLeft.y};
		vertices[1].texCoord = { topRight.x, botLeft.y };
		vertices[2].texCoord = { topRight.x, topRight.y };
		vertices[3].texCoord = { botLeft.x, topRight.y };
	}
	void SetColor(glm::vec4 color)
	{
		//for (size_t i = 0; i < vertices.size(); i++)
		//{
		//	vertices[i].color = color;
		//}
		this->color = color;
	}
	const std::shared_ptr<Texture> GetTexture() const { return texture; }
	const std::vector<Vertex>& GetVertices() const { return vertices; }
	const std::vector<uint32_t>& GetIndices() const { return indices; }
	const glm::vec4 GetColor() const { return color; }
private:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::shared_ptr<Texture> texture;
	glm::vec4 color = glm::vec4(1.f);
};