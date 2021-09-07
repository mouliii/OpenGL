#pragma once

#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "Mat3.h"
#include "Vec3.h"
#include "Vec2.h"
#include "Primitives.h"

#include "Renderer.h"

class Rect
{
public:
	Rect(Vec2f halfSize, glm::vec4 color = glm::vec4(1.0f))
		:
		quad(halfSize, color)
	{
		transform = glm::mat4(1.f);
	}
	Rect(Vec2f pos, Vec2f halfSize, glm::vec4 color = glm::vec4(1.0f))
		:
		quad(pos, halfSize, color)
	{
		transform = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, 0.0f));
	}
	void SetPosition(Vec2f pos)
	{
		transform = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, 0.0f));
	}
	void Translate(Vec2f offset)
	{
		transform = glm::translate(transform, glm::vec3(offset.x, offset.y, 0.0f));
	}
	void SetTextureUV(Vec2f botLeft, Vec2f topRight)
	{
		quad.GetVertices()[0].texCoord = { botLeft.x, botLeft.y };
		quad.GetVertices()[1].texCoord = { topRight.x, botLeft.y };
		quad.GetVertices()[2].texCoord = { topRight.x, topRight.y };
		quad.GetVertices()[3].texCoord = { botLeft.x, topRight.y };
	}
	void SetTextureIndex(float index)
	{
		for (size_t i = 0; i < quad.GetVertices().size(); i++)
		{
			quad.GetVertices()[i].textureIndex = index;
		}
	}
	// todo transform class tai jtn
	const glm::mat4& GetTransform() const { return transform; }
	Quad& GetPrimitive() { return quad; }
public:
	Quad quad;
	glm::mat4 transform;
};
