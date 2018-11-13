#ifndef CG_LAB3_MODEL_H
#define CG_LAB3_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "Shader.h"
#include "Mesh.h"
#include "Node.h"

class Model {
public:
    /*  Functions   */
    Model();
    Model(const char *path);
    void Draw(Shader shader);
    Node *getNode(string name);

    /* Data */
    Node *root;

private:
    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;

    /*  Functions   */
    void loadModel(const string path);
    void processNode(aiNode *node, const aiScene *scene, Node *currNode);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    mat4 convertToMat4(aiMatrix4x4 matrix);
    vec3 convertToVec3(aiColor3D color);
};


#endif //CG_LAB3_MODEL_H
