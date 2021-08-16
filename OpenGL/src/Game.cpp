#include "Game.h"

Game::Game(GLFWwindow* window)
	: 
    window(window),
    renderer(), camera(0.0f, windowWidth, 0.0f, windowHeight),
    rect1({ 50.f,50.f }, { 15.f,15.f }, {0.45f,0.5f,1.0f,1.0f}),
    rect2({ 300.f,400.f }, { 20.f,40.f }, { 0.4f,1.0f,0.2f,1.0f })
{
    game = new Pong();
}

Game::~Game()
{
    std::cout << "Destroying game class \n";

}

void Game::Run()
{
    renderer.ResetStats();
    renderer.Clear();
    renderer.BeginBatch();

    glm::mat4 viewProj = camera.GetViewProjectionMatrix();
    renderer.shader.SetUniform4fv("uViewProj", viewProj);
 

    Update();
    Draw();

    renderer.EndBatch();
    //
    ImGui::Begin("Test");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Mouse Coords x: %.0f, y: %.0f", mousePos.x, mousePos.y);
    ImGui::Text("Quads: %d Draw Calls: %d", renderer.GetRenderStats().quadCount, renderer.GetRenderStats().drawCalls);
    ImGui::End();
}

void Game::Update()
{
    const float curTime = float(glfwGetTime());
    const float dt = curTime - lastFrameTime;
    lastFrameTime = curTime;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    my = double(windowHeight) - my;
    mousePos = Vec2f(float(mx), float(my));

    //game->Update(window, dt);
    rect1.pos = mousePos;

    rect2.color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    if (Collision::AABB(rect1, rect2))
    {
        rect2.color = glm::vec4(0.4f, 1.0f, 0.2f, 1.0f);
    }
    Vec2f cp, cn;
    float t_hit;
    Rect rect3 = Rect(Vec2f(40.f, 250.f), { 10.f, 10.f }, { 0.4f,1.0f,0.2f,1.0f });
    rect3.Draw(renderer);
    if (Collision::RayVsRect(Vec2f(40.f, 250.f), Vec2f(mousePos.x - 40.f, mousePos.y - 250.f), rect2, &cp, &cn, &t_hit) && t_hit <= 1.0f)
    {
        rect2.color = glm::vec4(0.2f, 1.0f, 1.0f, 1.0f);
    }
}

void Game::Draw()
{
    //game->Draw(renderer);
    rect2.Draw(renderer);
    rect1.Draw(renderer);
}
