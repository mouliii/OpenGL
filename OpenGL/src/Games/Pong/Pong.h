#pragma once
#include "../BaseGame/BaseGame.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <random>
#include "../Renderer.h"
#include "../OrthoCamera.h"
#include "../Rectangle.h"
#include "box2d/box2d.h"

class Pong : public BaseGame
{
public:
	Pong();
	~Pong();
	void Update(GLFWwindow* window, float dt);
	void Draw(Renderer& gfx);
private:
	b2World* world;
	Rectangle paddleLeft;
	Rectangle paddleRight;
	Rectangle ball;
	float paddleSpeed = 20.f;
	float ballSpeed = 20.f;
	Vec2f ballDirection = { 0.0f,0.0f };
};