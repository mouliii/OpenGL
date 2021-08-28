#pragma once

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <random>
#include "OrthoCamera.h"

#include "Rect.h"
#include "Collision.h"
#include "Renderer.h"

/*
rectangle - cache vertices - bool update_pos

*/


class Game
{
public:
	Game(GLFWwindow* window);
	~Game();
	void Run();
private:
	void Update();
	void Draw();
private:
	GLFWwindow* window;
	const float windowWidth = 960;
	const float windowHeight = 540;
	Renderer gfx;
	OrthoCamera camera;
	Vec2<float> mousePos = { 0.0f,0.0f };
	float lastFrameTime = 0.0f;
	float dt = 0.0f;

public:
	Rect player;
	float speed = 200.f;
	Vec2f vel = {200.0f,200.0f};
	std::vector<Rect> rects;
};