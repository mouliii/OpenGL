#include "Tilemap.h"
#include <fstream>

Tilemap::Tilemap(const std::string& tiledDataPath, const std::string& tiledTileData, const std::string& tileMapTexture)
{
	// load texture
	const auto texture = TextureManager::LoadTexture(tileMapTexture);

	// load collision tile data
	nlohmann::json json2;
	std::ifstream instream2;
	instream2.open(tiledTileData);
	//instream2 >> json2;
	instream2.close();

	
	//struct ColArea
	//{
	//	float x = 0;
	//	float y = 0;
	//	float width = 0;
	//	float height = 0;
	//};
	//std::map<uint32_t, ColArea> collisionData;

	//for (size_t c = 0; c < json2["tiles"].size(); c++)
	//{
	//	ColArea area;
	//	const auto arrPath = json2["tiles"][c]["objectgroup"]["objects"][0];
	//
	//	uint32_t tileId = json2["tiles"][c]["id"];
	//	area.x = arrPath["x"];
	//	area.y = arrPath["y"];
	//	area.width = arrPath["width"];
	//	area.height = arrPath["height"];
	//
	//	collisionData.insert({ tileId, area});
	//}

	// read  map data
	// add collisions based on id
	nlohmann::json json;
	std::ifstream instream;
	instream.open(tiledDataPath);
	instream >> json;
	instream.close();

	mapWidthInTiles  =	json["width"];
	mapHeightInTiles =	json["height"];
	tileHeight		 =	json["tileheight"];
	tileWidth        =	json["tilewidth"];
	orientation      =	json["orientation"];
	renderOrder      =	json["renderorder"];


	for (size_t i = 0; i < json["layers"].size(); i++)
	{
		// cache
		int texWidthInTiles  = texture->GetWidth() / tileWidth;
		int texHeightInTiles = texture->GetHeight() / tileWidth;

		if (json["layers"][i]["type"] == "tilelayer")
		{
			std::vector<Tile> tileLayer;
			int index_x = 0;
			// invert y
			int index_y = mapHeightInTiles * tileHeight;
			Vec2f tileHalfSize = { tileWidth / 2.f, tileHeight / 2.f };

			const uint32_t layerCount = json["layers"][i]["data"].size();
			for (size_t j = 0; j < layerCount; j++)
			{
				// tile 0 - no tile, empty
				int id = json["layers"][i]["data"][j];
				
				if (id != 0)
				{
					// ids start at 0
					// offset id back to 0 ... max
					id--;
					
					Tile tile({ index_x + tileHalfSize.x, index_y - tileHalfSize.y }, tileHalfSize, tileMapTexture);
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

					const float padding = 1.f / 512.f;

					tile.GetRect().SetTextureIndex((float)texture->GetId());
					tile.GetRect().SetTextureUV({ uv_bl_x + padding, uv_bl_y + padding }, { uv_tr_x - padding, uv_tr_y - padding });
					tileLayer.push_back(tile);
					

					//// collision tiles
					//for (auto it = collisionData.begin(); it != collisionData.end(); it++)
					//{
					//	if (id == it->first)
					//	{
					//		// height = maxH - y
					//		// oikea_y = maxH - y - h
					//
					//		Vec2f botLeft((float)index_x, (float)index_y - tileHeight);
					//		// offset botleft
					//		botLeft += {it->second.x, tileHeight - it->second.y - it->second.height};
					//		// -> top right
					//		Vec2f topRight = {botLeft.x + it->second.width, botLeft.y + it->second.height};
					//		//collisionTiles.push_back({ { botLeft.x + (topRight.x-botLeft.x)*0.5f, botLeft.y + +(topRight.y - botLeft.y) * 0.5f },
					//		//	{ it->second.width*0.5f, it->second.height*0.5f },
					//		//	glm::vec4(1.f) });
					//		break;
					//	}
					//}
				}
				index_x += tileWidth;
				if (index_x % (mapWidthInTiles * tileWidth) == 0)
				{
					// inverted
					index_y -= tileHeight;
					index_x = 0;
				}
			}
			tiles.push_back(tileLayer);
		}
	}
}