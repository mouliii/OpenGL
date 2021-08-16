#pragma once
#include "../BaseGame/BaseGame.h"
#include <thread>
#include <mutex>
#include <random>
#include "box2d/box2d.h"
#include "../Vec2.h"
#include "../Renderer.h"
#include "../rectangle.h"

class Box2dTest : public BaseGame
{
public:
	Box2dTest();
	~Box2dTest();
	void Update(GLFWwindow* window, float dt);
	void Draw(Renderer& gfx);
	void UpdatePhysics();
private:
	std::thread physicsThread;
	std::mutex mutexPhysics;
	std::vector<Rectangle> rects;
	const float windowWidth = 960;
	const float windowHeight = 540;
	bool updatePhysics = true;
	float lastTime = 0.0f;
	float physFps = 0.0f;
	b2Vec2 gravity = { 0.0f, -10.0f };
	b2World* b2world;
	float physicsTimer = 0.0f;
	Vec2<float> mousePos = { 0.0f,0.0f };
};