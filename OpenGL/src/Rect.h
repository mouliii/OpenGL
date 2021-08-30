#pragma once

#include <iostream>
#include "Mat3.h"
#include "Vec3.h"
#include "Vec2.h"
#include "Primitives.h"

#include "Renderer.h"
#include "OrthoCamera.h"

class Rect
{
public:
	Rect(Vec2f centerPos, Vec2f halfSize, glm::vec4 color)
		:
		pos(centerPos), halfSize(halfSize), color(color),
		quad(centerPos, halfSize, color)
	{
		//localVertices.reserve(4);
		//localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 - halfSize.y));
		//localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 - halfSize.y));
		//localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 + halfSize.y));
		//localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 + halfSize.y));
	};

	void Draw(Renderer& renderer, const OrthoCamera& camera)
	{
		SetVertexPositions();
		renderer.Draw(quad, camera, texture.get() );
	}
	void Draw(Renderer& renderer, const OrthoCamera& camera, Texture* texture)
	{
		//auto asd = texture->GetID();
		renderer.Draw(quad, camera, texture );
	}
	void SetVertexPositions()
	{
		auto& verts = quad.GetVertices();
		verts[0].pos = Vec3f(Vec2f(pos.x - halfSize.x, pos.y - halfSize.y), 0.0f);
		verts[1].pos = Vec3f(Vec2f(pos.x + halfSize.x, pos.y - halfSize.y), 0.0f);
		verts[2].pos = Vec3f(Vec2f(pos.x + halfSize.x, pos.y + halfSize.y), 0.0f);
		verts[3].pos = Vec3f(Vec2f(pos.x - halfSize.x, pos.y + halfSize.y), 0.0f);
	}
	void LoadTexture(const std::string& path)
	{
		texture = TextureManager::GetTexture(path);
	}
public:
	//std::vector<Vec2f> localVertices;
	Quad quad;
	Vec2f pos;
	Vec2f halfSize;
	glm::vec4 color;
	std::shared_ptr<Texture> texture;
};
