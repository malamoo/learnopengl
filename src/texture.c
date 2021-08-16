#include "../external/glad/glad.h"
#include "../external/stb/stb_image.h"
#include "../include/texture.h"

/* Load an image to create a 2D texture. */
void texture_load(Texture *texture, const char *texture_path)
{
        unsigned char *data;
        unsigned int format;
        int width;
        int height;
        int channels;

        glGenTextures(1, &texture->id);
        stbi_set_flip_vertically_on_load(1);
        data = stbi_load(texture_path, &width, &height, &channels, 0);
        if (data) {
                if (channels == 1)
                        format = GL_RED;
                else if (channels == 3)
                        format = GL_RGB;
                else
                        format = GL_RGBA;
                glBindTexture(GL_TEXTURE_2D, texture->id);
                glTexImage2D(GL_TEXTURE_2D, 0, (int)format, width, height,
                             0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);
        } else {
                printf("Error: failed to load texture");
        }
        stbi_image_free(data);
}
