#pragma once
#include "json.hpp"
#include <vector>
#include "Vec2.h"
#include "glm/glm.hpp"
#include "Tile.h"
#include "TextureManager.h"

class Tilemap
{
public:
	Tilemap(const std::string& tiledDataPath, const std::string& tiledTileData, const std::string& tileMapTexture);
	~Tilemap()
	{
		for (size_t i = 0; i < tiles.size(); i++)
		{
			tiles[i].pop_back();
		}
	}
	std::vector<std::vector<Tile>>& GetTiles() { return tiles; }
	//const std::vector<Quad>& GetCollisionTiles() const { return collisionTiles; }
private:
	int mapWidthInTiles;
	int mapHeightInTiles;
	int tileHeight;
	int tileWidth;
	std::string orientation;
	std::string renderOrder;
	std::vector<std::vector<Tile>> tiles;
	//std::vector<Quad> collisionTiles;
};