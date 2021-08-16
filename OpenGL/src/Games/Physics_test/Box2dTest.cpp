#include "Box2dTest.h"
Box2dTest::Box2dTest()
{
    rects.emplace_back(Rectangle({ 500.f, 200.f }, { 500.f, 16.f }, glm::vec4(1.f, 0.8f, 0.4f, 1.f), b2world, b2_staticBody, 10));
    rects.emplace_back(Rectangle({ 10.f, 400.f }, { 20.f, 400.f }, glm::vec4(0.2f, 0.6f, 1.f, 1.f), b2world, b2_staticBody, 10));
    rects.emplace_back(Rectangle({ 930.f, 400.f }, { 20.f, 400.f }, glm::vec4(1.f, 0.6f, 1.f, 1.f), b2world, b2_staticBody, 10));
    rects.emplace_back(Rectangle({ 500.f, 500.f }, { 10.f, 10.f }, glm::vec4(1.f, 1.f, 1.f, 1.f), b2world, b2_dynamicBody, 10));
    rects.emplace_back(Rectangle({ 510.f, 400.f }, { 10.f, 10.f }, glm::vec4(0.6f, 1.f, 0.8f, 1.f), b2world, b2_dynamicBody, 10));

    std::default_random_engine dre;
    dre.seed((unsigned int)std::time(0));
    for (size_t i = 0; i < 900; i++)
    {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        rects.emplace_back(Rectangle({ 11.f + i, 600.f }, { 4.f, 4.f }, glm::vec4(dist(dre), dist(dre), dist(dre), 1.f), b2world, b2_dynamicBody, 10));
    }

    physicsThread = std::thread(&Box2dTest::UpdatePhysics, this);
}

Box2dTest::~Box2dTest()
{
}

void Box2dTest::Update(GLFWwindow* window, float dt)
{
    //double mx, my;
    //glfwGetCursorPos(window, &mx, &my);
    //my = double(windowHeight) - my;
    //mousePos = Vec2f(float(mx), float(my));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        //updatePhysics = false;
        //if (physicsThread.joinable())
        //{
        //    physicsThread.join();
        //}
    }

    int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (mouseState == GLFW_PRESS)
    {
        if (!b2world->IsLocked())
        {
            rects.emplace_back(Rectangle(mousePos, { 4.f, 4.f }, glm::vec4(0.5f, 0.6f, 0.7f, 1.f), b2world, b2_dynamicBody, 10));
        }
    }

    for (size_t i = 0; i < rects.size(); i++)
    {
        rects[i].Update();
    }
}

void Box2dTest::Draw(Renderer& gfx)
{
    for (size_t i = 0; i < rects.size(); i++)
    {
        rects[i].Draw(gfx);
    }
}

void Box2dTest::UpdatePhysics()
{
    std::cout << "UpdatePhysics: started\n";
    while (updatePhysics)
    {
        float timeStep = 1.0f / 60.0f;
        const float curTime = float(glfwGetTime());
        const float dt = curTime - physicsTimer;

        if (dt >= timeStep)
        {
            auto tn = std::chrono::high_resolution_clock::now();
            b2world->Step(timeStep, 6, 2); // 8,3
            physicsTimer = curTime;
            physFps = dt;
            std::this_thread::sleep_for(std::chrono::duration<float>(timeStep - dt));

            auto tl = std::chrono::high_resolution_clock::now();
        }
    }
    std::cout << "UpdatePhysics: end\n";
}
