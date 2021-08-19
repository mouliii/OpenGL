#include <glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "QuadRenderer.h"
#include "Batch.h"
#include "OrthoCamera.h"

constexpr int WINDOWWIDTH = 960;
constexpr int WINDOWHEIGHT = 540;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Opengl high severity message\n" << message << "\n";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Opengl medium severity message\n" << message << "\n";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Opengl low severity message\n" << message << "\n";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        //std::cout << "Opengl high severity message\n" << message << "\n";
        break;
    default:
        break;
    }
}

int main(void)
{
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
    // Enable error callbacks
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_BLEND);
    //glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // on windows resize
    glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
    // V-Sync
    glfwSwapInterval(1);

    //////////////////////////////////////////////////////
    Vertex vertData[] =
    {
        Vec3f(Vec2f(100.f, 100.f), 0.0f), glm::vec4(1.f, 0.f, 0.f, 1.f), Vec2f(0.0f, 0.0f),
        Vec3f(Vec2f(150.f, 100.f), 0.0f), glm::vec4(0.f, 1.f, 0.f, 1.f), Vec2f(1.0f, 0.0f),
        Vec3f(Vec2f(150.f, 150.f), 0.0f), glm::vec4(0.f, 0.f, 1.f, 1.f), Vec2f(1.0f, 1.0f),
        Vec3f(Vec2f(100.f, 150.f), 0.0f), glm::vec4(0.f, 0.f, 1.f, 1.f), Vec2f(0.0f, 1.0f)
    };
    std::vector<Vertex> vertices(vertData, vertData + sizeof(vertData) / sizeof(Vertex));
    std::vector<unsigned int> indices = {0u,1u,2u,0u,2u,3u};


    QuadRenderer r(vertices, indices);
    Shader shader("res/shaders/QuadVertex.shader", "res/shaders/QuadFragment.shader");


    OrthoCamera camera(0,WINDOWWIDTH,0,WINDOWHEIGHT);
    Batch batch(GL_TRIANGLES, "toimi", shader, 1000);
    batch.Add();
   
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Poll for and process events */
        glfwPollEvents();

        r.Draw(camera);
        batch.Draw(&shader, camera);

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
