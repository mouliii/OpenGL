#include "Tilemap.h"
#include <fstream>

Tilemap::Tilemap(const std::string& tiledDataPath, const std::string& tiledTileData, const std::string& tileMapTexture)
{
	// load texture
	texture = TextureManager::LoadTexture(tileMapTexture);
	// read  map data
	nlohmann::json json;
	std::ifstream instream(tiledDataPath);
	instream >> json;

	mapWidthInTiles  =	json["width"];
	mapHeightInTiles =	json["height"];
	tileHeight		 =	json["tileheight"];
	tileWidth        =	json["tilewidth"];
	orientation      =	json["orientation"];
	renderOrder      =	json["renderorder"];

	// todo render oreder

	for (size_t i = 0; i < json["layers"].size(); i++)
	{
		// cache
		int texWidthInTiles = texture->GetWidth() / tileWidth;
		int texHeightInTiles = texture->GetHeight() / tileWidth;

		if (json["layers"][i]["type"] == "tilelayer")
		{
			std::vector<Mesh> layerTiles;
			int index_x = 0;
			// invert y
			int index_y = mapHeightInTiles * tileHeight;
			Vec2f tileHalfSize = { tileWidth / 2.f, tileHeight / 2.f };

			for (size_t j = 0; j < json["layers"][i]["data"].size(); j++)
			{
				// tile 0 - no tile, empty
				int id = json["layers"][i]["data"][j];
				if (id != 0)
				{
					// ids start at 0
					// offset id back to 0 ... max
					id--;

					Quad rect({ index_x + tileHalfSize.x, index_y - tileHalfSize.y }, tileHalfSize, glm::vec4(1.f));
					Mesh mesh(rect);
					mesh.LoadTexture(tileMapTexture);
					//bot_left
					float cord_bl_x = (float)tileWidth * ((id) % texWidthInTiles);
					float cord_bl_y = (float)tileHeight * ((id) / texWidthInTiles) + tileHeight;
					cord_bl_y = texture->GetHeight() - cord_bl_y;
					
					float uv_bl_x = cord_bl_x  / float(texture->GetWidth());
					float uv_bl_y = cord_bl_y  / float(texture->GetHeight());
					//top_right
					float cord_tr_x = cord_bl_x + tileWidth;
					float cord_tr_y = cord_bl_y + tileHeight;

					float uv_tr_x = cord_tr_x  / float(texture->GetWidth());
					float uv_tr_y = cord_tr_y  / float(texture->GetHeight());

					mesh.SetTextureIndex((float)texture->GetId());
					const float padding = 1.f / 512.f;
					mesh.SetTextureUV({ uv_bl_x + padding, uv_bl_y + padding }, { uv_tr_x - padding, uv_tr_y - padding });
					layerTiles.push_back(mesh);
				}
				index_x += tileWidth;
				if (index_x % (mapWidthInTiles * tileWidth) == 0)
				{
					// inverted
					index_y -= tileHeight;
					index_x = 0;
				}
			}
			tiles.push_back(layerTiles);
		}
	}
}