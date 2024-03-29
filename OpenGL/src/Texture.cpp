#include "Texture.h"

Texture::Texture(const std::string& filepath, GLenum type)
{
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &nChannels, 0);
    if (data)
    {
        glGenTextures(1, &id);
        glBindTexture(type, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // GL_NEAREST
        switch (nChannels)
        {
        case 3:
            glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            std::cout << "Texture channel unsupported! :" << nChannels << " channels" << std::endl;
            break;
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Bind() const
{
    //glBindTextureUnit(slot, id); TOIMII VAIN GL 4.5 + !
    glBindTexture(type, id);
}

void Texture::Unbind() const
{
    glBindTexture(0, id);
}

void Texture::Delete() const
{
    glDeleteTextures(1, &id);
}

const uint32_t Texture::GetId() const
{
    return id;
}
