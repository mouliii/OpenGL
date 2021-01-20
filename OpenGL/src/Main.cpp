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
    // V-Sync
    glfwSwapInterval(1);

    // INIT GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "glew init failed" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float positions[] = {
        100.f, 100.f, 0.0f, 0.0f,
        200.f, 100.f, 1.0f, 0.0f,
        200.f, 200.f, 1.0f, 1.0f,
        100.f, 200.f, 0.0f, 1.0f
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

    glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.f, 0.f, 0.f));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(300.f, 200.f, 0.f));

    glm::mat4 mvp = proj * view * model;

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.5f, 1.0f, 1.0f);

    Texture texture("res/textures/plul.jpg");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);
    shader.SetUniformMat4f("u_MVP", mvp);
    Renderer renderer;

    // error check pre loop
    errorCheck.GLLogError();

    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    // rect väri
    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.5f, 1.0f, 1.0f);

        renderer.Draw(va, ib, shader);

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

        // error check TODO 
        errorCheck.GLLogError();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}