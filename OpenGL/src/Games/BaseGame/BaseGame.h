#pragma once
#include "../Renderer.h"
#include "GLFW/glfw3.h"

class BaseGame
{
public:
	BaseGame() = default;
	virtual void Update(GLFWwindow* window, float dt){}
	virtual void Draw(Renderer& gfx){}
private:
};