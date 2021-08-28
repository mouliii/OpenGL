#include "Game.h"

Game::Game(GLFWwindow* window)
	: 
    window(window),
    camera(0.0f, windowWidth, 0.0f, windowHeight),
    player(Vec2f(300.f, 300.f), Vec2f(20.f, 30.f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
    for (size_t i = 0; i < 20; i++)
    {
        rects.emplace_back(Vec2f(400.f + i*60, 100.f), Vec2f(10.f, 10.f), glm::vec4(0.4f, 0.5f, 0.45f, 1.0f));
    }
    rects.emplace_back(Vec2f(400.f + 1 * 60, 150.f), Vec2f(10.f, 10.f), glm::vec4(0.4f, 0.5f, 0.45f, 1.0f));
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
    Shader s("res/shaders/DefaultVertex.shader", "res/shaders/DefaultFragment.shader");
    s.Bind();
    glm::mat4 viewProj = camera.GetViewProjectionMatrix();
    s.SetUniform4fv("uViewProj", viewProj);

    Vec2f dir = { 0.0f,0.0f };
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        dir.x = -1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        dir.x = 1;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        dir.y = 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        dir.y = -1;
    dir.Normalize();
    vel = dir * speed;

    Rect q(Vec2f(300.f, 400.f), Vec2f(15.f, 30.f), glm::vec4(0.4f, 0.3f, 0.78f, 1.0f));
    Rect q2({ 270.f, 360.f }, { 15.f, 30.f }, { 0.4f, 0.3f, 0.78f, 1.0f });
    Line l(player.pos, Vec2f(mousePos), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), 1);

    Vec2f cp, cn;
    float t;

    if (Collision::RayVsRect(player.pos, mousePos - player.pos, q, &cp, &cn, &t) && t <= 1.0f)
        q.quad.SetVertexColor(glm::vec4(0.6f, 0.3f, 0.28f, 1.0f));
    else
        q.quad.SetVertexColor(glm::vec4(0.4f, 0.3f, 0.78f, 1.0f));

    // first pass
    float tSort = 0;
    std::vector<std::pair<size_t, float>> z;
    for (size_t i = 0; i < rects.size(); i++)
    {
        
        // collision distance
        if (Collision::DynamicRectVsRect(player, vel, rects[i], &cp, &cn, &tSort, dt))
        {
            z.push_back({ i,tSort });
        }
    }
    //sort col dist
    std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) { return a.second < b.second;});


    // second pass
    for (auto& j : z)
    {
        if (Collision::DynamicRectVsRect(player, vel, rects[j.first], &cp, &cn, &t, dt))
        {
            player.pos = cp;
            float dotProduct = (vel.x * cn.y + vel.y * cn.x) * (1.f - t);
            vel.x = dotProduct * cn.y;
            vel.y = dotProduct * cn.x;
        }
    }

    for (size_t i = 0; i < 2; i++)
    {
        if (Collision::DynamicRectVsRect(player, vel, i == 0 ? q : q2, &cp, &cn, &t, dt))
        {
            player.pos = cp;
            float dotProduct = (vel.x * cn.y + vel.y * cn.x) * (1.f - t);
            vel.x = dotProduct * cn.y;
            vel.y = dotProduct * cn.x;
        }
    }

    player.pos += vel * dt;
    player.SetVertexPositions();
  

    gfx.Draw(q.quad, camera);
    gfx.Draw(player.quad, camera);
    gfx.Draw(l, camera);

    gfx.Draw(q2.quad, camera);

    for (size_t i = 0; i < rects.size(); i++)
    {
        gfx.Draw(rects[i].quad, camera);
    }

    ImGui::Begin("Test");
    ImGui::Text("player pos: %.0f, %.0f ", player.pos.x, player.pos.y);
    ImGui::Text("player vel: %.1f, %.1f ", vel.x, vel.y);
    ImGui::End();
}
