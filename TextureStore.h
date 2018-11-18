#ifndef CG_PROJECT_TEXTURESTORE_H
#define CG_PROJECT_TEXTURESTORE_H

#include <map>
#include <GL/gl.h>

using namespace std;

class TextureStore {
public:
    GLuint loadTexture (const char* path, string name);
    GLuint getTexture (string name);
    static TextureStore *instance ();
private:
    TextureStore(){};
    static TextureStore *store_instance;
    map<string,GLuint> textures;
};


#endif //CG_PROJECT_TEXTURESTORE_H
