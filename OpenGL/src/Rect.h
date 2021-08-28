#pragma once

#include <iostream>
#include "Mat3.h"
#include "Vec3.h"
#include "Vec2.h"

#include "Batch.h"

class Rect
{
public:
	Rect(Vec2f centerPos, Vec2f halfSize, glm::vec4 color, Batch* batch = nullptr)
		:
		pos(centerPos), halfSize(halfSize), color(color), batch(batch),
		quad(centerPos, halfSize, color)
	{
		//localVertices.reserve(4);
		//localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 - halfSize.y));
		//localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 - halfSize.y));
		//localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 + halfSize.y));
		//localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 + halfSize.y));
	};

	void Draw()
	{
		//std::vector<Vec2f> globalVertices = localVertices;
		//
		//globalVertices[0] += Vec2f(pos.x, pos.y);
		//globalVertices[1] += Vec2f(pos.x, pos.y);
		//globalVertices[2] += Vec2f(pos.x, pos.y);
		//globalVertices[3] += Vec2f(pos.x, pos.y);

		//batch->Update(vertices);
	}
	void SetVertexPositions()
	{
		auto& verts = quad.GetVertices();
		verts[0].pos = Vec3f(Vec2f(pos.x - halfSize.x, pos.y - halfSize.y), 0.0f);
		verts[1].pos = Vec3f(Vec2f(pos.x + halfSize.x, pos.y - halfSize.y), 0.0f);
		verts[2].pos = Vec3f(Vec2f(pos.x + halfSize.x, pos.y + halfSize.y), 0.0f);
		verts[3].pos = Vec3f(Vec2f(pos.x - halfSize.x, pos.y + halfSize.y), 0.0f);
	}

public:
	//std::vector<Vec2f> localVertices;
	Quad quad;
	Vec2f pos;
	Vec2f halfSize;
	glm::vec4 color;
	Batch* batch;
};
