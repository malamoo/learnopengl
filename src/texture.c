#include "../external/glad.h"
#include "../external/stb_image.h"
#include "../include/texture.h"

/* Makes a texture from the specified image file. */
unsigned int make_texture(char *name)
{
        unsigned int texture;
        unsigned char *data;
        int width;
        int height;
        int channels;
        int format;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(1);
        data = stbi_load(name, &width, &height, &channels, 0);
        if (data) {
                if (channels == 4)
                        format = GL_RGBA;
                else
                        format = GL_RGB;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                             0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
        } else {
                printf("Error: failed to load texture");
        }
        stbi_image_free(data);
        return texture;
}
