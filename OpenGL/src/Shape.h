#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "../Renderer.h"
#include <iostream>
#include "Mat3.h"
#include "Vec3.h"
#include "Vec2.h"

enum class ShapeType{RECTANGLE, CIRCLE, POLYGON};

class Shape
{
public:
	Shape(Vec2f pos, std::vector<Vec2<float>> vertices, glm::vec4 color)
		:
		pos(pos), vertices(vertices), color(color)
	{
	}
	void Draw(Renderer& renderer)
	{
		std::vector<Vec2f> globalVertices = vertices;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			globalVertices[i].x += pos.x;
			globalVertices[i].y += pos.y;
		}
		renderer.DrawQuad(globalVertices, color);
	}

	void Update(float dt)
	{

	}

	void Translate(const Vec2f& offset)
	{
		pos += offset;
	}

	void SetPostition(const Vec2f& pos)
	{
		this->pos = pos;
	}

	void SetScale(float scale)
	{
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].x /= this->scale;
			vertices[i].y /= this->scale;

			vertices[i].x = vertices[i].x * scale;
			vertices[i].y = vertices[i].y * scale;
		}
		this->scale = scale;
	}

	void Rotation(float rotation)
	{
		for (size_t i = 0; i < vertices.size(); i++)
		{
			//vertices[i] = Mat3f::Rotate(rotation) * vertices[i];
			vertices[i].Rotate(rotation);
		}
		this->rotation += rotation;
		ClampRotation();
	}

	void SetRotation(float rotation)
	{
		float target_rotation = (rotation - this->rotation);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			//vertices[i] = Mat3f::Rotate(target_rotation) * vertices[i];
			vertices[i].Rotate(target_rotation);
		}
		this->rotation = rotation;
		ClampRotation();
	}

	void ClampRotation()
	{
		const float pi = 3.141f;
		if (rotation > (pi))
		{
			const float temp = rotation - pi;
			rotation = (-pi) + temp;
		}
		else if (rotation < -pi)
		{
			const float temp = rotation + pi;
			rotation = pi + temp;
		}
	}

	void CenterOrigin()
	{
		// todo
	}

public:
	std::vector<Vec2<float>> vertices;
	Vec2f pos;
	float rotation = 0.f;
	float scale = 1.0f;
	glm::vec4 color;
	ShapeType type = ShapeType::POLYGON;
};