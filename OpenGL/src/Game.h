#pragma once

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <random>
#include "OrthoCamera.h"
#include "Renderer.h"
#include "Collision.h"

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
	Renderer renderer;
	OrthoCamera camera;
	Shader shader;
	Vec2<float> mousePos = { 0.0f,0.0f };
	float lastFrameTime = 0.0f;
	float dt = 0.0f;


};