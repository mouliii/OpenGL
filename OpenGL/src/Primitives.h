#pragma once

#include <vector>
#include "Vec2.h"
#include "Vec3.h"
#include "glm/glm.hpp"
#include "VertexData.h"

class Primitive
{
public:
	Primitive(){}
	virtual ~Primitive(){}
	const std::vector<uint32_t>& GetIndices() const { return indices; }
	const std::vector<Vertex>& GetVertices() const { return vertices; }
	const uint32_t GetVertexCount() const { return vertices.size() * sizeof(uint32_t) / sizeof(Vertex); } // bugaa
	const uint32_t GetIndexCount() const { return indices.size(); }
protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};


// batch ottaa primitiven

class Quad : public Primitive
{
public:
	Quad()
		:Primitive()
	{
		vertices.reserve(4);
		vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f() });
		vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f() });
		vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f() });
		vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f() });
		indices = { 0,1,2, 2,3,0 };
	}
	Quad(Vec2f centerPos, Vec2f halfSize, glm::vec4 color)
		:Primitive()
	{
		vertices.reserve(4);
		vertices.push_back({ Vec3f(Vec2f(centerPos.x - halfSize.x, centerPos.y - halfSize.y), 0.0f), color, Vec2f(0.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x + halfSize.x, centerPos.y - halfSize.y), 0.0f), color, Vec2f(1.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x + halfSize.x, centerPos.y + halfSize.y), 0.0f), color, Vec2f(1.0f, 1.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x - halfSize.x, centerPos.y + halfSize.y), 0.0f), color, Vec2f(0.0f, 1.0f) });

		indices = { 0,1,2, 2,3,0 };
	}
private:
};

class Triangle : public Primitive
{
public:
	Triangle(Vec2f centerPos, Vec2f halfSize, glm::vec4 color)
		:Primitive()
	{
		vertices.reserve(4);
		vertices.push_back({ Vec3f(Vec2f(centerPos.x - halfSize.x, centerPos.y - halfSize.y), 0.0f), color, Vec2f(0.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x + halfSize.x, centerPos.y - halfSize.y), 0.0f), color, Vec2f(1.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x, centerPos.y + halfSize.y), 0.0f), color, Vec2f(1.0f, 1.0f) });

		indices = {0,1,2};
	}
private:
};