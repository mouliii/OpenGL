#include "Game.h"

#include "tilemap/Tilemap.h"

Game::Game(GLFWwindow* window)
	: 
    window(window),
    camera(0.0f, windowWidth, 0.0f, windowHeight),
    renderer(&camera),
    map("res/levels/level1/test_map.json", "todo", "res/levels/level1/Tileset.png"),
    batch("tilemap", Quad())
{
    for (size_t i = 0; i < map.GetTiles().size(); i++)
    {
        for (size_t j = 0; j < map.GetTiles()[i].size(); j++)
        {
            batch.Add(1, Quad());
        }
    }
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

    ImGui::Begin("Test");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Mouse Coords x: %.0f, y: %.0f", mousePos.x, mousePos.y);
    ImGui::Text("camera pos: x: %.0f, y: %.0f", camera.position.x, camera.position.y);
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

    MoveCamera(dt);
    camera.RecalculateViewMatrix();

}

void Game::Draw()
{
    batch.cam = &camera;
    batch.BeginBatch();
    for (size_t i = 0; i < map.GetTiles().size(); i++)
    {
        for (size_t j = 0; j < map.GetTiles()[i].size(); j++)
        {
            //renderer.Draw(map.GetTiles()[i][j]);
            const auto& verts = map.GetTiles()[i][j].GetVertices();
            batch.Update(verts, map.GetTiles()[i][j].GetTexture());
        }
    }
    batch.EndBatch();
    batch.Flush();
    
    // imgui
    ImGui::Begin("Test");
    //ImGui::Text("player pos: %.0f, %.0f ", player.pos.x, player.pos.y);
    ImGui::End();
}

void Game::MoveCamera(float dt)
{
    Vec2f dir = { 0.0f,0.0f };
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        dir.x -= 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        dir.x += 1;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        dir.y += 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        dir.y -= 1;
    dir.Normalize();
    camera.position += glm::vec3(cameraSpeed * dir.x * dt, cameraSpeed * dir.y * dt, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.scale -= 0.1f * dt;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.scale += 0.1f * dt;

}
