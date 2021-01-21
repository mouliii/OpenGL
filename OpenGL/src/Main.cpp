#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GLErrorCheck.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../test/rectTest.h"


int main(void)
{

    GLFWwindow* window;
    GLErrorCheck errorCheck;
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    errorCheck.ClearErrors();

    // V-Sync
    glfwSwapInterval(1);

    // INIT GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "glew init failed" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float positions[] = {
       -50.f, -50.f, 0.0f, 0.0f,
        50.f, -50.f, 1.0f, 0.0f,
        50.f, 50.f, 1.0f, 1.0f,
       -50.f, 50.f, 0.0f, 1.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    


    // vao
    VertexArray va;
    // bindaa gldatabufferiin data | vbo
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    // tekee kaiken tarvittavan, että bufferi toimii | "vao välikäsi"
    VertexBufferLayout layout;
    // 2 floattia eli [x,y]
    layout.Push<float>(2);
    layout.Push<float>(2);
    // bindaa koko paska vao
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices));

    glm::vec3 translation(400.0f, 400.0f, 0.0f);
    float rotation(0.f);
    float scale = 1.0f;

    glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
    glm::mat4 model;


    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    //Texture texture("res/textures/plul.jpg");
    //texture.Bind();
    //shader.SetUniform1i("u_Texture", 0);
    Renderer renderer;
    // error check pre loop
    errorCheck.GLLogError();

    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();
    
    
    std::vector<testRect> rects;
    for (size_t i = 0; i < 15; i++)
    {
        testRect rect;
        rects.push_back(rect);
    }

    // rect väri
    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        model = glm::translate(glm::mat4(1.0f), translation) *
                glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
 
        glm::mat4 mvp = proj * view * model;

        /* Render here */
        renderer.Clear();
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.5f, 1.0f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);

        for (size_t i = 0; i < rects.size(); i++)
        {
            glm::mat4 modelpos(1.0f);
            modelpos = glm::translate(glm::mat4(1.0f), rects[i].pos);
            glm::mat4 mvp = proj * view * modelpos;
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.5f, 1.0f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);
            rects[i].Draw(renderer, shader);
        }

        if (r > 1.0f)
        {
            r = 1.0f;
            increment = -0.005f;
        }
        else if (r < 0.0f)
        {
            r = 0.0f;
            increment = 0.005f;
        }
        r += increment;

        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat3("Translation", &translation.x, 0.f, 960.f);
        ImGui::SliderFloat("Scale", &scale, 0.0f, 3.0f);
        ImGui::SliderFloat("Rotation", &rotation, -3.14f, 3.14f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // error check TODO 
        errorCheck.GLLogError();
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