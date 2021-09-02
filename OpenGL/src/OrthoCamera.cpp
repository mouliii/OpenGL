#include "OrthoCamera.h"
#include "glm/gtc/matrix_transform.hpp"

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
	:
	projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
	viewMatrix(1.0f),
	cameraDimensions(left, right, bottom, top)
{
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void OrthoCamera::RecalculateViewMatrix()
{
	glm::mat4 transfrom = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1)) *
		glm::scale(glm::mat4(1.f),glm::vec3(scale,scale,1));
	viewMatrix = glm::inverse(transfrom);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void OrthoCamera::SetPosition(glm::vec2 position)
{
	this->position = glm::vec3(position, 0.0f);
	RecalculateViewMatrix();
}

void OrthoCamera::SetRotation(float rotation)
{
	this->rotation = rotation;
	RecalculateViewMatrix();
}

void OrthoCamera::Zoom(float zoomLevel)
{
	scale = zoomLevel;
}
