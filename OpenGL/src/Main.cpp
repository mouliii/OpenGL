
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"

#include "stb/stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define WINDOWWIDTH 960
#define WINDOWHEIGHT 540

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 2.5f;
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
double lastX = 800.0f / 2.0;
double lastY = 600.0 / 2.0;

float deltaTime = 0.0f;
float lastTime = 0.0f;

bool cursorMode = 0;
float mouseSensitivity = 0.1f;
// perspectivessä ei ole "screen spacea". 1 on metri       orthossa mapataan screeniin.
glm::mat4 proj = glm::perspective(glm::radians(90.0f), float(WINDOWWIDTH) / float(WINDOWHEIGHT), 0.1f, 100.0f);
//glm::mat4 proj = glm::ortho(0.f, width, 0.f, height, -1.0f, 100.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_BLEND);
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
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        // clearing 1280 error imgui ????
    }
    float vertices[] = {
//      x    y      z    r     g     b     a   texture coords
     -50.f, -50.f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
      50.f, -50.f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
      50.f, 50.f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  // bottom left
     -50.f, 50.f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f // top left 
    };

    float verts[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     -0.5f,-0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    // myötäpäivään!
    unsigned int indices[] = {  // note that we start from 0!
       0, 1, 2,   // first triangle
       2, 3, 0    // second triangle
    };

    Texture texture1("res/textures/plul.jpg");
    Texture texture2("res/textures/awesomeface.png");

    VertexArray vao;
    VertexBuffer vbo(verts, sizeof(verts) );
    IndexBuffer ibo(indices, 6);

    BufferLayout layout(
        {
            {DataType::FLOAT, 3, "uPosition"},
            {DataType::FLOAT, 4, "uColor"},
            {DataType::FLOAT, 2, "uTexCoord"}
        }
    );
    
    vbo.SetLayout(layout);
    // adaa layoutin attributet vao
    vao.AddVertexBuffer(vbo);

    Shader shader("res/shaders/VertexShader.shader", "res/shaders/FragmentShader.shader");
    shader.Bind();
    shader.SetUniform1i("uTexture", 0);
    shader.SetUniform1i("uTexture2", 1);

    // error check
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "OpenGL warning/error - pre loop: " << err << std::endl;
    }
    // imgui
    float alphaVal = 0.0f;
    // model
    float scale = 1.0f;
    glm::vec3 rotation(0.0, 0.0f, 0.0f);; // -glm::pi<float>() / 2
    glm::vec3 translation(0.0, 0.0f, 1.0f);
    glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::translate(glm::mat4(1.0f), translation) *
                glm::rotate(glm::mat4(1.0f), rotation[0], glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), rotation[1], glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), rotation[2], glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));

    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 mvp = proj * view * transform;
    // glDraw
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    vao.Unbind();
    vbo.Unbind();
    shader.Unbind();

    glActiveTexture(GL_TEXTURE0);
    texture1.Bind();
    glActiveTexture(GL_TEXTURE1);
    texture2.Bind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // delta time
        const float currentTime = float(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        ProcessInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        transform = glm::translate(glm::mat4(1.0f), translation) *
            glm::rotate(glm::mat4(1.0f), rotation[0], glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), rotation[1], glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), rotation[2], glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);     
        mvp = proj * view * transform; // * local coords

        shader.Bind();
        vao.Bind();
        ibo.Bind();


        shader.SetUniform1f("uAlpha", alphaVal);
        shader.SetUniform4fv("uMVP", mvp);

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

        GLenum err;
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
           
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("Alpha value", &alphaVal, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat3("Transform", &translation[0], -1.0f, 1.0f);
            ImGui::SliderFloat("Scale", &scale, 0.0f, 3.0f);
            ImGui::SliderFloat3("Rotation", &rotation[0], -3.14f, 3.14f);
            ImGui::SliderFloat("tranZ", &translation[2], -10.0f, 10.0f);
            ImGui::Text("CamX %.3f CamY %.3f CamZ %.3f", cameraPos.x, cameraPos.y, cameraPos.z);
            ImGui::Text("CamYaw %.3f CamPitch %.3f", yaw, pitch);
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

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float camSpeed = cameraSpeed * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += camSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= camSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos.y += camSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos.y -= camSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cursorMode = !cursorMode;
        if (cursorMode)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cursorMode)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += float(xoffset);
        pitch += float(yoffset);

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        yaw = glm::mod(yaw + float(xoffset), 360.0f);

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }
    
}
