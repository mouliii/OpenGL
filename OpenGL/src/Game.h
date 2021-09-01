#pragma once

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <random>
#include "OrthoCamera.h"
#include "Renderer.h"
#include "Collision.h"

#include "tilemap/Tilemap.h"

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
	OrthoCamera camera;
	Renderer renderer;
	Vec2<float> mousePos = { 0.0f,0.0f };
	float lastFrameTime = 0.0f;
	float dt = 0.0f;

public:
	Tilemap map;
	float cameraSpeed = 150.f;
public:
	void MoveCamera(float dt);
};