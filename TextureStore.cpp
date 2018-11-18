#include <iostream>
#include <GL/glew.h>
#include "TextureStore.h"
#include "stb_image/stb_image.h"


GLuint TextureStore::loadTexture(const char* path, string name) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    GLuint texID;
    if (data)
    {
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture " << path << std::endl;
    }
    stbi_image_free(data);

    this->textures.insert(make_pair(name,texID));
    return texID;
}

GLuint TextureStore::getTexture(string name) {
    return this->textures[name];
}
TextureStore *TextureStore::store_instance = 0;

TextureStore *TextureStore::instance() {
    if (!store_instance) {
        store_instance = new TextureStore();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
    }
    return store_instance;
}