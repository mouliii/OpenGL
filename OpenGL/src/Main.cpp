#include <glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OrthoCamera.h"

constexpr int WINDOWWIDTH = 960;
constexpr int WINDOWHEIGHT = 540;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void glfwSetMouseButtonCallback(GLFWwindow* window,int a, int b, int c); // todo

int main(void)
{
    GLenum err;

    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    //glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // on windows resize
    glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
    // V-Sync

    glfwSwapInterval(1);
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        //std::cout << "err pre verts:  " << err << std::endl;
    }

    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "clear imgui error ?:  " << err << std::endl;
    }
    //////////////////////////////////////////////////////
    Vertex vertData[] =
    {
        Vec3f(Vec2f(100.f, 100.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(0.0f, 0.0f),
        Vec3f(Vec2f(100.f, 150.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(1.0f, 0.0f),
        Vec3f(Vec2f(150.f, 150.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(1.0f, 1.0f),
        Vec3f(Vec2f(100.f, 150.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(0.0f, 1.0f)
    };

    std::vector<Vertex> vertices;
    vertices.push_back(vertData[0]);
    vertices.push_back(vertData[1]);
    vertices.push_back(vertData[2]);
    vertices.push_back(vertData[3]);
    //vertices.emplace_back(Vec3f(Vec2f(100.f, 100.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(0.0f, 0.0f));
    //vertices.emplace_back(Vec3f(Vec2f(100.f, 150.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(1.0f, 0.0f));
    //vertices.emplace_back(Vec3f(Vec2f(150.f, 150.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(1.0f, 1.0f));
    //vertices.emplace_back(Vec3f(Vec2f(100.f, 150.f), 0.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), Vec2f(0.0f, 1.0f));

    std::vector<unsigned int> indices = {0,1,2,0,2,3};
    //////////////////////////////////////////////////////
    VertexArray vao;
    VertexBuffer vbo(vertices);
    IndexBuffer ibo(indices);
    Shader shader("res/shaders/QuadVertex.shader", "res/shaders/QuadFragment.shader");

    vao.Bind();
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "err :  " << err << std::endl;
    }
    vbo.Bind();
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "err :  " << err << std::endl;
    }
    ibo.Bind();
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "err :  " << err << std::endl;
    }
    vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
    vao.LinkAttribute(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "err " << err << std::endl;
    }

    OrthoCamera camera(0,WINDOWWIDTH,0,WINDOWHEIGHT);
   
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Poll for and process events */
        glfwPollEvents();

        shader.Bind();
        glm::mat4 viewProj = camera.GetViewProjectionMatrix();
        shader.SetUniform4fv("uViewProj", viewProj);
        vao.Bind();

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "err " << err << std::endl;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
