#pragma once
#include "glm/glm.hpp"
#include <iostream>
#include "Mat3.h"
#include "Vec3.h"
#include "Vec2.h"

#include "VertexData.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OrthoCamera.h"

class Rect
{
public:
	Rect(Vec2f centerPos, Vec2f halfSize, glm::vec4 color)
		:
		pos(centerPos), halfSize(halfSize), color(color)
	{
		localVertices.reserve(4);
		localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 - halfSize.y));
		localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 - halfSize.y));
		localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 + halfSize.y));
		localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 + halfSize.y));

	};

	void Draw(Shader shader, OrthoCamera cam)
	{
		std::vector<Vec2f> globalVertices = localVertices;

		globalVertices[0] += Vec2f(pos.x, pos.y);
		globalVertices[1] += Vec2f(pos.x, pos.y);
		globalVertices[2] += Vec2f(pos.x, pos.y);
		globalVertices[3] += Vec2f(pos.x, pos.y);

	}


public:
	std::vector<Vec2f> localVertices;
	//Vertex vertData[4];
	Vec2f pos;
	Vec2f halfSize;
	glm::vec4 color;

};
