#pragma once
#include "Vec2.h"
#include "glm/gtc/matrix_transform.hpp"
#include "TextureManager.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"


class Player
{
public:
	Player(Primitive prim, Vec2f pos, const std::string& texturePath)
		:
		mesh(prim), pos(pos), shader("res/shaders/NewVertex.shader", "res/shaders/NewFragment.shader")
	{
		mesh.LoadTexture(texturePath);
		//transform = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, 0.0f));
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
	}
	void Draw(Renderer& renderer)
	{
		transform = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, 0.0f));
		renderer.Draw(mesh, mesh.GetTexture().get(), transform, shader);
		//std::cout << transform[3].x << " " << transform[3].y << std::endl;
	}
	const Vec2f GetPostion() const { return pos; }
private:
	Mesh mesh;
	Vec2f pos;
	glm::mat4 transform = glm::mat4(1.f);
	Vec2f vel = {0.0f,0.0f};
	float speed = 220.f;

	Shader shader;
};