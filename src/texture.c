#include "../ext/glad.h"
#include "../ext/stb_image.h"
#include "../include/texture.h"

/* Makes a texture from the specified image file. */
unsigned int maketexture(char *name)
{
        unsigned int texture;
        unsigned char *data;
        int width;
        int height;
        int channels;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        data = stbi_load(name, &width, &height, &channels, 0);
        if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                             GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
        } else {
                printf("Error: failed to load texture");
        }
        stbi_image_free(data);
        return texture;
}
