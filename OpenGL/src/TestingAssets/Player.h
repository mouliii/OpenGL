#pragma once
#include "Vec2.h"
#include "glm/gtc/matrix_transform.hpp"
#include "TextureManager.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"
#include "box2d/box2d.h"
#include "Box2dConstants.h"

#include "Rect.h"

class Player
{
public:
	Player(Vec2f pos, Vec2f halfSize, const std::string& texturePath, b2World* world)
		:
		pos(pos), halfSize(halfSize), shader("res/shaders/NewVertex.shader", "res/shaders/NewFragment.shader"),
		rect(halfSize), pTexture(std::make_shared<Texture>(texturePath))
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x / Box2dC::pixelPerMeter, pos.y / Box2dC::pixelPerMeter);
		body = world->CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(halfSize.x / Box2dC::pixelPerMeter, halfSize.y / Box2dC::pixelPerMeter);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 1.0f;

		body->CreateFixture(&fixtureDef);
		body->SetFixedRotation(true);

	}
	void Update(GLFWwindow* window, float dt)
	{
		Vec2f dir = { 0.0f,0.0f };
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			dir.x -= 1;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			dir.x += 1;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			dir.y += 1;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			dir.y -= 1;
		dir.Normalize();
		vel = dir * speed;
		pos += vel * dt;
		rect.SetPosition(pos);

		body->SetLinearVelocity(b2Vec2(vel.x / Box2dC::pixelPerMeter, vel.y / Box2dC::pixelPerMeter));

		//std::cout << body->GetPosition().x * Box2dC::pixelPerMeter << "  " << body->GetPosition().y * Box2dC::pixelPerMeter << std::endl;
	}
	void Draw(Renderer& renderer)
	{
		transform = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, 0.0f));
		renderer.Draw(rect.GetPrimitive(), pTexture.get(), transform, shader);
	}

	Rect& GetRect() { return rect; }
	const Vec2f GetPostion() const { return pos; }
	Vec2f& GetVelocity()  { return vel; }
	const Vec2f GetHalfSize()const { return halfSize; }

	Vec2f pos;
	b2Body* body;
private:
	Vec2f halfSize;
	Rect rect;
	std::shared_ptr<Texture> pTexture;
	glm::mat4 transform = glm::mat4(1.f);
	Vec2f vel = {0.0f,0.0f};
	float speed = 220.f;

	Shader shader;

};