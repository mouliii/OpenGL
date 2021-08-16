#include "Pong.h"

Pong::Pong()
	:
	world(new b2World(b2Vec2(0.0f, 0.0f))),
	paddleLeft(Rectangle({ 50.f, 200.f }, { 16.f, 32.f }, glm::vec4(1.f, 0.8f, 0.4f, 1.f), world, b2_kinematicBody, 10)),
	paddleRight(Rectangle({ 910.f, 200.f }, { 16.f, 32.f }, glm::vec4(1.f, 0.8f, 0.4f, 1.f), world, b2_kinematicBody, 10)),
	ball(Rectangle({ 960.f*0.5f, 540.f * 0.5f}, { 10.f, 10.f }, glm::vec4(1.f, 1.f, 1.f, 1.f), world, b2_kinematicBody, 10))
{
	std::default_random_engine dre;
	dre.seed((unsigned int)std::time(0));
	std::uniform_real<float> dist(0.0f, 1.0f);
	paddleLeft.color = {dist(dre), dist(dre), dist(dre), 1.0f };
	paddleRight.color = { dist(dre), dist(dre), dist(dre), 1.0f };

	if (dist(dre) > 0.5f)
	{
		ballDirection.x = 1.0f;
	}
	else
	{
		ballDirection.x = -1.0f;
	}
	std::uniform_real<float> bally(-1.0f, 1.0f);
	ballDirection.y = bally(dre);

}

Pong::~Pong()
{
	delete world;
}

void Pong::Update(GLFWwindow* window, float dt)
{
	Vec2f dir(0.f, 0.f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dir.y += 1.f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dir.y -= 1.f;

	paddleLeft.Translate(dir * paddleSpeed);

	if (ball.pos.y < paddleRight.pos.y)
	{
		paddleRight.Translate(Vec2f(0.0f, -1.0f) * paddleSpeed);
	}
	else
	{
		paddleRight.Translate(Vec2f(0.0f, 1.0f) * paddleSpeed);
	}

	if (ball.pos.y + ball.halfSize.y > 540.f) {
		ballDirection.y *= -1;
		ball.pos.y = 540.f-ball.halfSize.y-10.f;
	}
	else if (ball.pos.y - ball.halfSize.y < 0.0f) {
		ballDirection.y *= -1;
		ball.pos.y = 0.f + ball.halfSize.y+10.f;
	}
	std::cout << ballDirection.y << "\n";

	for (b2ContactEdge* edge = ball.body->GetContactList(); edge; edge = edge->next)
	{
		bool collision = edge->contact->IsTouching();
		if (collision)
			ballDirection.x *= -1;
	}
	ball.Translate(ballDirection * ballSpeed);

	world->Step(1.f / 144.f, 6, 2);

	ImGui::Begin("Pong");
	ImGui::Text("paddle left linear vel y: %f.0f", paddleLeft.body->GetLinearVelocity().y);
	ImGui::Text("paddle left pos y: %f.0f", paddleLeft.pos.y);
	ImGui::End();
}

void Pong::Draw(Renderer& gfx)
{
	paddleLeft.Draw(gfx);
	paddleRight.Draw(gfx);
	ball.Draw(gfx);
}
