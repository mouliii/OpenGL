#pragma once
#include "glm/glm.hpp"

class OrthoCamera
{
public:

	OrthoCamera(float left, float right, float bottom, float top);
	void RecalculateViewMatrix();
	void SetPosition(glm::vec2 position);
	void SetRotation(float rotation);
	void Zoom(float zoomLevel);
	const glm::mat4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;
	glm::vec4 cameraDimensions;
public:
	glm::vec3 position = {0.0f,0.0f,0.0f};
	float rotation = 0.0f;
	float scale = 1.0f;
};