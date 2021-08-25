#include <glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Primitives.h"
#include "Batch.h"
#include "OrthoCamera.h"
#include "Rect.h"
#include "TextureManager.h"

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

    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_BLEND);
    //glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // on windows resize
    glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
    // V-Sync
    glfwSwapInterval(1);

    //Setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    Renderer renderer;
    Shader shader("res/shaders/DefaultVertex.shader", "res/shaders/DefaultFragment.shader");
    OrthoCamera camera(0,WINDOWWIDTH,0,WINDOWHEIGHT);
    Texture texture("res/textures/plul.jpg");
    //Texture texture("res/textures/white1x1.png");
    {
        auto asdTest = TextureManager::GetTexture("res/textures/awesomeface.png");
    }
    std::vector<Quad> quads;
    std::vector<Triangle> tris;
    std::vector<Line> lines;

    lines.emplace_back(Vec2f(0.0f, 0.0f), Vec2f(900.f, 500.f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 2);
    lines.emplace_back(Vec2f(0.0f, 540.0f), Vec2f(925, 0.f), glm::vec4(0.0f, 1.0f, 0.4f, 1.0f), 1);
    lines.emplace_back(Vec2f(300.0f, 230.0f), Vec2f(600, 230.f), glm::vec4(0.3f, 0.9f, 0.9f, 1.0f), 5);
    std::vector<Vec2f> pline{Vec2f(200.f,100.f), Vec2f(600.f,100.f), Vec2f(400.f, 500.f)};
    lines.emplace_back(pline, glm::vec4(1.0f,1.0f,1.0f,1.0f), 3, true);

    int insertTri = 0;
    for (size_t x = 10; x < 950; x+=5)
    {
        for (size_t y = 10; y < 540; y+=5)
        {
            //if ( insertTri % 1 == 0)
            //    quads.emplace_back(Quad(Vec2f(float(x), float(y)), Vec2f(2.f, 2.f), glm::vec4(1.0f, 0.5f, 0.3f, 0.4f)));
            if (insertTri % 20 == 0)
            {
                tris.emplace_back(Vec2f(float(x), float(y)), Vec2f(6.f, 6.f), glm::vec4(0.05f, 1.0f, 0.3f, 0.5f));
            }
            insertTri += 1;
        }
    }
    for (size_t x = 10; x < 950; x += 13)
    {
        for (size_t y = 10; y < 540; y += 13)
        {
            quads.emplace_back(Vec2f(float(x), float(y)), Vec2f(5.f, 5.f), glm::vec4(1.0f, 0.5f, 0.3f, 0.4f));
        }
    }
    Shader batchShader("res/shaders/DefaultBatchVertex.shader", "res/shaders/DefaultBatchFragment.shader");
    //                                            30k -> 4.6 MB | max 4
    Batch quadBatch(GL_FILL, "quads", batchShader, quads[0]);
    quadBatch.Add(quads.size(), quads[0]);
    Batch triangleBatch(GL_LINE, "triangles", batchShader, tris[0]);
    triangleBatch.Add(tris.size(), tris[0]);
    Batch lineBatch(GL_FILL, "lines", batchShader, lines[0], 100);
    // eri kokoiset linet pit‰‰ laittaa erikseen
    lineBatch.Add(lines.size()-1, lines[0]);
    lineBatch.Add(1, lines.back());

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    TextureManager::FreeUnusedTextures();
    std::shared_ptr<uint32_t> testTex = TextureManager::GetTexture("res/textures/awesomeface.png");
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Poll for and process events */
        glfwPollEvents();

        for (size_t i = 0; i < quads.size(); i++)
        {
            quadBatch.Update(quads[i]);
        }
        for (size_t i = 0; i < tris.size(); i++)
        {
            triangleBatch.Update(tris[i]);
        }
        for (size_t i = 0; i < lines.size(); i++)
        {
            lineBatch.Update(lines[i]);
        }
        quadBatch.Draw(camera);
        triangleBatch.Draw(camera);
        lineBatch.Draw(camera);

        renderer.Draw(Quad(Vec2f(70.f, 400.f), Vec2f(20.f, 20.f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), camera, testTex);
        renderer.Draw(Triangle(Vec2f(700.f, 400.f), Vec2f(30.f, 30.f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), camera, &texture);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");                          
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("number of rects: %d", quads.size());
        ImGui::Text("Draw calls: %d", quadBatch.numOfDrawCalls);
        ImGui::End();

        // Rendering
        ImGui::Render();
        //int display_w, display_h;
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
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
