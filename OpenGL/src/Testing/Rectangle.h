#pragma once

#include "glm/glm.hpp"
#include "../Renderer.h"

class Rectangle
{
public:
	Rectangle(glm::vec3 pos, glm::vec4 color)
		:
		pos(pos), color(color)
	{}
	void Draw(Renderer& renderer)
	{
		renderer.DrawQuad(pos, glm::vec2(32.0f, 32.0f), color);
	}
private:
	glm::vec3 pos;
	glm::vec4 color;
};