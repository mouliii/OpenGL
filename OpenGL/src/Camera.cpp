#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float Yaw, float Pitch)
	:
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
{
    Position = position;
    WorldUp = up;
    this->Yaw = Yaw;
    this->Pitch = Pitch;
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float Yaw, float Pitch)
    : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = Yaw;
    this->Pitch = Pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::GetCameraPosition()
{
    return glm::vec3(Position.x, Position.y, Position.z);
}

glm::vec2 Camera::GetCameraAxis()
{
    return glm::vec2(Yaw, Pitch);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        Position.y += velocity;
    if (direction == BACKWARD)
        Position.y -= velocity;
    if (direction == LEFT)
        Position.x -= velocity;
    if (direction == RIGHT)
        Position.x += velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when Pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}
