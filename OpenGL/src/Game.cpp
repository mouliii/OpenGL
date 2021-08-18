#include "Game.h"
#include "Line.h"

Game::Game(GLFWwindow* window)
	: 
    window(window),
    camera(0.0f, windowWidth, 0.0f, windowHeight)
{
    Rect rect(Vec2f(300.f, 300.f), Vec2f(50.f, 50.f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

Game::~Game()
{
    std::cout << "Destroying game class \n";
}

void Game::Run()
{
    glClearColor(0.07f, 0.13f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    

    glm::mat4 viewProj = camera.GetViewProjectionMatrix();
    //renderer.shader.SetUniform4fv("uViewProj", viewProj);
 
    Update();
    Draw();

    //
    ImGui::Begin("Test");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Mouse Coords x: %.0f, y: %.0f", mousePos.x, mousePos.y);
    //ImGui::Text("Quads: %d Draw Calls: %d", renderer.GetRenderStats().quadCount, renderer.GetRenderStats().drawCalls);
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

    Shader s("res/shaders/QuadVertex.shader", "res/shaders/QuadFragment.shader");
    //
    
    //rect.Draw(s, camera);
    

}

void Game::Draw()
{

    //Line line(Vec2f(0.f, 0.f), Vec2f(960.f, 540.f));
    //line.Draw(&camera);
}
