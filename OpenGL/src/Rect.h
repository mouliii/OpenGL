#pragma once

#include <iostream>
#include "Mat3.h"
#include "Vec3.h"
#include "Vec2.h"

#include "Batch.h"

class Rect
{
public:
	Rect(Vec2f centerPos, Vec2f halfSize, glm::vec4 color, Batch* batch)
		:
		pos(centerPos), halfSize(halfSize), color(color), batch(batch)
	{
		//localVertices.reserve(4);
		//localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 - halfSize.y));
		//localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 - halfSize.y));
		//localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 + halfSize.y));
		//localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 + halfSize.y));
		vertices.reserve(4);
		vertices.push_back({ Vec3f(Vec2f(centerPos.x - halfSize.x, centerPos.y - halfSize.y), 0.0f), color, Vec2f(0.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x + halfSize.x, centerPos.y - halfSize.y), 0.0f), color, Vec2f(1.0f, 0.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x + halfSize.x, centerPos.y + halfSize.y), 0.0f), color, Vec2f(1.0f, 1.0f) });
		vertices.push_back({ Vec3f(Vec2f(centerPos.x - halfSize.x, centerPos.y + halfSize.y), 0.0f), color, Vec2f(0.0f, 1.0f) });

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


public:
	//std::vector<Vec2f> localVertices;
	std::vector<Vertex> vertices;
	Vec2f pos;
	Vec2f halfSize;
	glm::vec4 color;
	Batch* batch;
};
