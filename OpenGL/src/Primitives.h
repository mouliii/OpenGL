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
	const uint32_t GetVertexCount() const { return vertices.size(); }
	const uint32_t GetIndexCount() const { return indices.size(); }
protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

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
		vertices.push_back({ Vec3f(Vec2f(centerPos.x, centerPos.y + halfSize.y), 0.0f), color, Vec2f(0.5f, 1.0f) });

		indices = {0,1,2};
	}
private:
};

class Line : public Primitive
{
public:
	Line(Vec2f from, Vec2f to, glm::vec4 color, uint32_t size)
		:Primitive()
	{
		vertices.reserve(4);
		vertices.push_back({ Vec3f(Vec2f(from.x, from.y - size), 0.0f), color, Vec2f(0.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(to.x, to.y - size), 0.0f), color, Vec2f(1.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(to.x, to.y + size), 0.0f), color, Vec2f(1.0f, 1.0f) });
		vertices.push_back({ Vec3f(Vec2f(from.x, from.y + size), 0.0f), color, Vec2f(0.0f, 1.0f) });
		indices = { 0,1,2, 2,3,0 };
	}
	Line(std::vector<Vec2f> points, glm::vec4 color, uint32_t size, bool closeLine = false)
		:Primitive()
	{
		for (size_t i = 0; i < points.size() - 1; i++)
		{
			vertices.push_back({ Vec3f(Vec2f(points[i].x, points[i].y - size), 0.0f), color, Vec2f(0.0f, 0.0f) });
			vertices.push_back({ Vec3f(Vec2f(points[i + 1].x, points[i + 1].y - size), 0.0f), color, Vec2f(1.0f, 0.0f) });
			vertices.push_back({ Vec3f(Vec2f(points[i + 1].x, points[i + 1].y + size), 0.0f), color, Vec2f(1.0f, 1.0f) });
			vertices.push_back({ Vec3f(Vec2f(points[i].x, points[i].y + size), 0.0f), color, Vec2f(0.0f, 1.0f) });
		}
		int offset = 0;
		for (size_t i = 0; i < points.size() - 1; i++)
		{
			indices.push_back(offset + 0);
			indices.push_back(offset + 1);
			indices.push_back(offset + 2);
			indices.push_back(offset + 2);
			indices.push_back(offset + 3);
			indices.push_back(offset + 0);
			offset += 4;
		}
		if (closeLine)
		{
			vertices.push_back({ Vec3f(Vec2f(points.back().x, points.back().y - size), 0.0f), color, Vec2f(0.0f, 0.0f) });
			vertices.push_back({ Vec3f(Vec2f(points.front().x, points.front().y - size), 0.0f), color, Vec2f(1.0f, 0.0f) });
			vertices.push_back({ Vec3f(Vec2f(points.front().x, points.front().y + size), 0.0f), color, Vec2f(1.0f, 1.0f) });
			vertices.push_back({ Vec3f(Vec2f(points.back().x, points.back().y + size), 0.0f), color, Vec2f(0.0f, 1.0f) });
			indices.push_back(offset + 0);
			indices.push_back(offset + 1);
			indices.push_back(offset + 2);
			indices.push_back(offset + 2);
			indices.push_back(offset + 3);
			indices.push_back(offset + 0);
		}
	}
private:

};