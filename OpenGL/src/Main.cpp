
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"

#include "stb/stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Testing/Rectangle.h"

#define WINDOWWIDTH 960
#define WINDOWHEIGHT 540

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOWWIDTH / 2.0f;
float lastY = WINDOWHEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastTime = 0.0f;

bool cursorMode = 1;
float mouseSensitivity = 0.1f;
// perspectivessä ei ole "screen spacea". 1 on metri       orthossa mapataan screeniin.
//glm::mat4 proj = glm::perspective(glm::radians(90.0f), float(WINDOWWIDTH) / float(WINDOWHEIGHT), 0.1f, 100.0f);
glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.0f, 100.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);

int main(void)
{

    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // on windows resize
    glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
    // V-Sync
    glfwSwapInterval(0);

    //Setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        // clearing 1280 error imgui ????
    }
    Renderer renderer;
    Texture texture("res/textures/awesomeface.png");
    Rectangle rect(glm::vec3(300.f,300.f, 0.0f), glm::vec4(1.0f, 0.5f, 0.25f, 1.0f));
    Rectangle rect2(glm::vec3(200.f, 300.f, 0.0f), glm::vec4(0.5f, 1.0f, 0.25f, 1.0f));
    
    // error check
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "OpenGL warning/error - pre loop: " << err << std::endl;
    }
   
    // model
    float scale = 1.0f;
    float rotation(0.0f); // -glm::pi<float>() / 2
    glm::vec3 translation(0.0, 0.0f, 0.0f);
    glm::mat4 transform = glm::mat4(1.0f);

    glm::mat4 view = camera.GetViewMatrix();

    // glDraw
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // delta time
        const float currentTime = float(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        ProcessInput(window);
        /* RESET RENDER STATS*/
        renderer.ResetStats();
        renderer.Clear();
        renderer.BeginBatch();

        transform = glm::translate(glm::mat4(1.0f), translation);
        // view
        view = camera.GetViewMatrix();
        glm::mat4 viewProj = proj * view * transform;
        renderer.shader.SetUniform4fv("uViewProj", viewProj);

        for (float y = 100.f; y < 500.f; y += 20.f)
        {
            for (float x = 100.f; x < 500.f; x += 20.f)
            {
                glm::vec4 color = { (x + 10.f) / 20.f, 0.2f, (y + 10.f) / 20.f, 1.0f };
                renderer.DrawQuad({ x,y,0.0f }, { 15.f, 15.f }, color);
            }
        }
        rect.Draw(renderer);
        rect2.Draw(renderer);

        renderer.DrawQuad(glm::vec3(150.f, 150.f, 0.f), glm::vec2(64.f, 64.f), texture );

        renderer.EndBatch();

        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "OpenGL warning/error: " << err << std::endl;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {

            ImGui::Begin("Hello, world!");
            ImGui::Text("Quads: %d Draw Calls: %d", renderer.GetRenderStats().quadCount, renderer.GetRenderStats().drawCalls );
            ImGui::SliderFloat3("Transform", &translation[0], -100.0f, 100.0f);
            ImGui::SliderFloat3("Rotation", &rotation, -3.14f, 3.14f);
            ImGui::Text("CamX %.3f CamY %.3f CamZ %.3f", camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);
            ImGui::Text("CamYaw %.3f CamPitch %.3f", camera.GetCameraAxis().x, camera.GetCameraAxis().y);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }

    float xoffset = float(xpos) - lastX;
    float yoffset = lastY - float(ypos); // reversed since y-coordinates go from bottom to top

    lastX = float(xpos);
    lastY = float(ypos);

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(float(yoffset));
}