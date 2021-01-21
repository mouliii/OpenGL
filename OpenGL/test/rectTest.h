#pragma once

#include "../Renderer.h"
#include "glm/glm.hpp"
#include <random>

class testRect
{
public:
	testRect();
	void Draw(const Renderer& renderer, const Shader& shader) const;
public:
	glm::vec3 pos;

};


