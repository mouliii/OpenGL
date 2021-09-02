#pragma once
#include "json.hpp"
#include <vector>
#include "Vec2.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include "TextureManager.h"
#include "Batch.h"

class Tilemap
{
public:
	Tilemap(const std::string& tiledDataPath, const std::string& tiledTileData, const std::string& tileMapTexture);
	std::vector<std::vector<Mesh>>& GetTiles() { return tiles; }
private:
	std::shared_ptr<Texture> texture;
	int mapWidthInTiles;
	int mapHeightInTiles;
	int tileHeight;
	int tileWidth;
	std::string orientation;
	std::string renderOrder;
	std::vector<std::vector<Mesh>> tiles;
};