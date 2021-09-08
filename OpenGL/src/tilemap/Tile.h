#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "Vec2.h"
#include "TextureManager.h"
#include "Rect.h"
#include "Renderer.h"

class Tile
{
public:
	Tile(Vec2f center, Vec2f halfSize, const std::string& texturePath)
		:
		rect(center,halfSize), pos(center), pTexture(TextureManager::LoadTexture(texturePath))
	{
		transform = glm::translate(glm::mat4(1.f), glm::vec3(center.x, center.y, 0.0f));
	}
	void Draw(Renderer& renderer)
	{
		// todo batch rendering
		//renderer.Draw(rect.GetPrimitive(), pTexture.get(), )
	}
	Rect& GetRect() { return rect; }
	std::shared_ptr<Texture> GetTexture() { return pTexture; }
private:
	Rect rect;
	Vec2f pos;
	std::shared_ptr<Texture> pTexture;
	glm::mat4 transform;
};