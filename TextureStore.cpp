#include <iostream>
#include <GL/glew.h>
#include "TextureStore.h"
#include "stb_image/stb_image.h"


GLuint TextureStore::loadTexture(const char* path, string name, int textureUnit) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    GLuint texID;
    if (data)
    {
        bool alpha = nrChannels == 4;
        glGenTextures(1, &texID);
        glActiveTexture(GL_TEXTURE0+texID);
        cout<<"loading "<<name<<" into "<<texID<<endl;
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, (alpha ? GL_RGBA : GL_RGB), width, height, 0, (alpha ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture " << path << std::endl;
    }
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    this->textures.insert(make_pair(name,texID));
    return texID;
}

GLuint TextureStore::loadCubeMap(const char *paths[], string name) {
    stbi_set_flip_vertically_on_load(false);
    int width, height, nrChannels;
    unsigned char *data;
    GLuint texID;
    glActiveTexture(GL_TEXTURE9);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    for (int i=0; i<6; i++) {
        data = stbi_load(paths[i], &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        else
        {
            std::cout << "Failed to load texture " << paths[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_set_flip_vertically_on_load(true);
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
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
    }
    return store_instance;
}
