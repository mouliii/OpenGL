#pragma once
#include "Vec2.h"
#include "glm/gtc/matrix_transform.hpp"
#include "TextureManager.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"

#include "Rect.h"


class Player
{
public:
	Player(Vec2f pos, Vec2f halfSize, const std::string& texturePath)
		:
		pos(pos), halfSize(halfSize), shader("res/shaders/NewVertex.shader", "res/shaders/NewFragment.shader"),
		rect(halfSize), pTexture(std::make_shared<Texture>(texturePath))
	{
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

private:
	Vec2f pos;
	Vec2f halfSize;
	Rect rect;
	std::shared_ptr<Texture> pTexture;
	glm::mat4 transform = glm::mat4(1.f);
	Vec2f vel = {0.0f,0.0f};
	float speed = 220.f;

	Shader shader;
};