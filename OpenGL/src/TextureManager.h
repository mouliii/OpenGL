#pragma once

#include <unordered_map>
#include <iostream>
#include "stb/stb_image.h"
#include "glad/glad.h"
#include "Texture.h"


class TextureManager
{
public:
    TextureManager(const TextureManager&) = delete;
    static TextureManager& Get()
    {
        static TextureManager instance;
        return instance;
    }
    static std::shared_ptr<Texture> GetTexture(const std::string& path) { return Get().I_GetTexture(path); }
    static void FreeUnusedTextures() { Get().I_FreeUnusedTextures(); }
private:
    TextureManager() {};
	std::shared_ptr<Texture> I_GetTexture(const std::string& path)
	{
		size_t position = path.find_last_of('/')+ 1;
		const std::string name = path.substr(position, path.size() - position);

		const auto i = texturePtrs.find(name);
		if (i != texturePtrs.end())
		{
			return i->second;
		}
		else
		{
			auto pTex = std::make_shared<Texture>(path);
            texturePtrs.insert({ name, pTex });
			return pTex;
		}
	}
    void I_FreeUnusedTextures()
    {
        for (auto i = texturePtrs.begin(); i != texturePtrs.end(); )
        {
            if (i->second.unique())
            {
                i = texturePtrs.erase(i);
            }
            else
            {
                ++i;
            }
        }
    }
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> texturePtrs;
};
