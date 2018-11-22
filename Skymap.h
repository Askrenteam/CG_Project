#ifndef CG_PROJECT_SKYMAP_H
#define CG_PROJECT_SKYMAP_H


#include "Model.h"

class Skymap : public Mesh {
public:
    GLuint textureID;
    Skymap(const char* paths[]);
    void Draw(Shader shader);
private:
    void setupMesh();
};


#endif //CG_PROJECT_SKYMAP_H
