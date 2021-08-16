#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "../Renderer.h"
#include <iostream>
#include "Mat3.h"
#include "Vec3.h"
#include "Vec2.h"
#include "box2d/box2d.h"

class Rectangle
{
public:
	Rectangle(Vec2f centerPos, Vec2f halfSize, glm::vec4 color, b2World* world, b2BodyType type, float pixelsPerMeterScale)
		:
		pos(centerPos), halfSize(halfSize), color(color), ppm(pixelsPerMeterScale)
	{
		localVertices.reserve(4);
		localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 - halfSize.y));
		localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 - halfSize.y));
		localVertices.emplace_back(Vec2f(0 + halfSize.x, 0 + halfSize.y));
		localVertices.emplace_back(Vec2f(0 - halfSize.x, 0 + halfSize.y));

		b2BodyDef bodyDef;
		bodyDef.type = type;
		bodyDef.position.Set(centerPos.x / pixelsPerMeterScale, centerPos.y / pixelsPerMeterScale);
		body = world->CreateBody(&bodyDef);
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(halfSize.x / pixelsPerMeterScale, halfSize.y / pixelsPerMeterScale);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		body->CreateFixture(&fixtureDef);
	};

	void Draw(Renderer& renderer)
	{
		std::vector<Vec2f> globalVertices = localVertices;

		globalVertices[0] += Vec2f(pos.x, pos.y);
		globalVertices[1] += Vec2f(pos.x, pos.y);
		globalVertices[2] += Vec2f(pos.x, pos.y);
		globalVertices[3] += Vec2f(pos.x, pos.y);

		renderer.DrawQuad(globalVertices, color);
	}

	void Update()
	{
		pos = Vec2<float>(body->GetPosition().x * ppm, body->GetPosition().y * ppm);
		SetRotation(body->GetAngle());
	}

	void Translate(Vec2f velocity)
	{
		pos = Vec2<float>(body->GetPosition().x * ppm, body->GetPosition().y * ppm);
		body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	}

	void SetScale(float scale)
	{
		for (size_t i = 0; i < localVertices.size(); i++)
		{
			localVertices[i].x /= this->scale;
			localVertices[i].y /= this->scale;
	
			localVertices[i].x = localVertices[i].x * scale;
			localVertices[i].y = localVertices[i].y * scale;
		}
		this->scale = scale;
	}
	
	void Rotation(float rotation)
	{
		for (size_t i = 0; i < localVertices.size(); i++)
		{
			localVertices[i] = Mat3f::Rotate(rotation) * localVertices[i];
		}
		this->rotation += rotation;
		ClampRotation();
	}

	void SetRotation(float rotation)
	{
		float target_rotation = (rotation - this->rotation);
		for (size_t i = 0; i < localVertices.size(); i++)
		{
			localVertices[i] = Mat3f::Rotate(target_rotation) * localVertices[i];
		}
		this->rotation = rotation;
		ClampRotation();
	}

	void ClampRotation()
	{
		const float pi = 3.141f;
		//const int nTimes = int(rotation / pi);
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

public:
	std::vector<Vec2f> localVertices;
	Vec2f pos;
	Vec2f halfSize;
	Vec2f velocity = { 0.0f, 0.0f };
	float rotation = 0.f;
	float scale = 1.0f;
	float speed = 100.f;
	glm::vec4 color;
	// b2d
	float ppm = 0.0f;
	b2Body* body = nullptr;
};


/*
		Vec2f dir(0.f,0.f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			dir.y += 1.f;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			dir.y -= 1.f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			dir.x -= 1.f;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			dir.x += 1.f;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			//SetScale(2.0f);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			//SetScale(1.0f);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			//Rotation(.01f);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			//Rotation(-0.01f);
		dir.Normalize();

		velocity = dir * dt * speed;
		pos += velocity;
*/