#pragma once

#include <unordered_map>
#include <iostream>
#include "stb/stb_image.h"
#include "glad/glad.h"


class TextureManager
{
public:
    TextureManager(const TextureManager&) = delete;
    static TextureManager& Get()
    {
        static TextureManager instance;
        return instance;
    }
    static std::shared_ptr<uint32_t> GetTexture(const std::string& path) { return Get().I_GetTexture(path); }
    static void FreeUnusedTextures() { Get().I_FreeUnusedTextures(); }
private:
    TextureManager() {};
	std::shared_ptr<uint32_t> I_GetTexture(const std::string& path)
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
            uint32_t id = I_LoadTexture(path);
			auto pTex = std::make_shared<uint32_t>(id);
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
                std::cout << "deleted " << i->first << std::endl;
                i = texturePtrs.erase(i);
            }
            else
            {
                ++i;
            }
        }
    }
	uint32_t I_LoadTexture(const std::string& path)
	{
        int width, height, nChannels;
        uint32_t id;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
        if (data)
        {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // GL_NEAREST
            switch (nChannels)
            {
            case 3:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                break;
            default:
                std::cout << "Texture channel unsupported! :" << nChannels << " channels" << std::endl;
                break;
            }
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
            return id;
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        return -1;
	}
private:
	std::unordered_map<std::string, std::shared_ptr<uint32_t>> texturePtrs;
};

//std::unordered_map<std::string, std::shared_ptr<uint32_t>> TextureManager::texturePtrs;