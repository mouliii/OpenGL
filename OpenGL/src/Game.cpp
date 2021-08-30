#include "Game.h"

Game::Game(GLFWwindow* window)
	: 
    window(window),
    camera(0.0f, windowWidth, 0.0f, windowHeight),
    shader("res/shaders/DefaultVertex.shader", "res/shaders/DefaultFragment.shader")
{
}

Game::~Game()
{
    std::cout << "Destroying game class \n";
}

void Game::Run()
{
    glClearColor(0.07f, 0.13f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Update();
    Draw();

    //
    ImGui::Begin("Test");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Mouse Coords x: %.0f, y: %.0f", mousePos.x, mousePos.y);
    //ImGui::Text("Quads: %d Draw Calls: %d", renderer.GetRenderStats().quadCount, renderer.GetRenderStats().drawCalls);
    ImGui::End();

    ImGui::Render();
    //int display_w, display_h;
    //glfwGetFramebufferSize(window, &display_w, &display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::Update()
{
    const float curTime = float(glfwGetTime());
    dt = curTime - lastFrameTime;
    lastFrameTime = curTime;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    my = double(windowHeight) - my;
    mousePos = Vec2f(float(mx), float(my));

}

void Game::Draw()
{
    shader.Bind();
    glm::mat4 viewProj = camera.GetViewProjectionMatrix();
    shader.SetUniform4fv("uViewProj", viewProj);

    Rect rect({ 100.f,100.f }, { 100.f ,100.f }, { 1.0f,0.0f ,1.0f ,1.0f });
    //rect.LoadTexture("res/textures/container.jpg");
    std::shared_ptr<Texture> t =  TextureManager::GetTexture("res/textures/container.jpg");

    rect.Draw(renderer, camera, t.get());
    // imgui
    ImGui::Begin("Test");
    //ImGui::Text("player pos: %.0f, %.0f ", player.pos.x, player.pos.y);
    ImGui::End();
}
