#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 200.f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera
{
public:
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float Yaw = YAW, float Pitch = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float Yaw, float Pitch);
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    // camera coords xyz
    glm::vec3 GetCameraPosition();
    glm::vec2 GetCameraAxis();
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
private:
    void UpdateCameraVectors();
private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float fov;
};